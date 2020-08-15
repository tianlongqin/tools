#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <thread_pool.h>

#include <Tatomic.h>

#define T_SUCCESS 0
#define T_ENOMEM -1
#define T_EINVAL -2
#define T_EFAULT -3

#ifndef CONFIG_THREADPOOL_THREAD_MAX
#define TPOOL_THREAD_MAX 64
#else
#define TPOOL_THREAD_MAX CONFIG_THREADPOOL_THREAD_MAX
#endif

#define TBUF_MAX TPOOL_THREAD_MAX
typedef void *(*task_func)(void *);

struct work {
	unsigned char 	core;
	Tatomic_u32	task_num;
	int		run;
};

struct pools {
	unsigned char 	cores;
	unsigned int 	num;

	int 		fd[TBUF_MAX][2];
	pthread_t 	ptd[TBUF_MAX];

	int 		nw;
	struct work	works[TBUF_MAX];
};

struct task {
	task_func 	func;
	void 		*arg;
};


static void *working(void *arg);
int Tthreadpool_create(void **__pool, unsigned int num)
{
	int rc, i;
	struct pools *pool = NULL;

	if (num > TBUF_MAX) {
		rc = T_EINVAL;
		goto err;
	}

	pool = calloc(1, sizeof(*pool));
	if (!pool) {
		rc = T_ENOMEM;
		goto err;
	}

	pool->cores = sysconf(_SC_NPROCESSORS_ONLN);
	pool->num = num;

	for (i = 0; i < num; i++) {
		rc = pipe(pool->fd[i]);
		if (rc < 0) {
			perror("pipe failed");
			goto err;
		}
		pool->nw = i;
		pool->works[i].core = i;

		pthread_create(&pool->ptd[i], NULL, working, pool);
		usleep(10 * 1000);
	}


	*__pool = pool;

	return T_SUCCESS;
err:
	for (i = 0; i < num; i++) {
		if (pool && pool->fd[i]) {
			close(pool->fd[i][0]);
			close(pool->fd[i][1]);
		}
	}

	if (pool)
		free(pool);

	return rc;
}

void Tthreadpool_loop(void *_pool)
{
	int num, i;
	pthread_t id;
	struct pools *pool = _pool;
	if (!pool)
		return ;

	num = pool->num;
	for (i = 0; i < num; i++) {
		id = pool->ptd[i];
		pthread_join(id, NULL);
	}

}
void Tthreadpool_destroy(void **_pool)
{
	int num, i;
	pthread_t id;
	struct pools *pool = *_pool;
	if (!pool)
		return ;

	num = pool->num;
	for (i = 0; i < num; i++) {
		id = pool->ptd[i];
		pool->works[i].run = 0;
		Tthreadpool_add_task(pool, i, NULL, NULL);

		if (pool->fd[i]) {
			close(pool->fd[i][0]);
			close(pool->fd[i][1]);
		}
	}

	free(pool);
	*_pool = NULL;
}

int Tthreadpool_add_task(void *_pool, int core_id, task_func function, void *arg)
{
	struct pools *pool = _pool;
	struct work *work = &pool->works[core_id];
	struct task task;
	int *fd = pool->fd[core_id];
	int rc = T_SUCCESS;

	task.func = function;
	task.arg = arg;

	rc = write(fd[1], &task, sizeof(task));
	if (rc < 0 || rc != sizeof(task)) {
		rc = T_EFAULT;
		goto err;
	}

	Tatomic_addf(&work->task_num, 1);
err:

	return rc;
}

int Tthreadpool_get_minid(void *_pool)
{
	struct pools *pool = _pool;
	struct work *work;
	int i, num = pool->num;
	int min = 0, min_num = 0;

	for (i = 0; i < num; i++) {
		work = &pool->works[i];
		if (i == 0) {
			min = Tatomic_loadn(&work->task_num);
			continue;
		}

		if (min > Tatomic_loadn(&work->task_num)) {
			min = Tatomic_loadn(&work->task_num);
			min_num = i;
		}
	}

	return min_num;
}

static void *working(void *arg)
{
	struct pools *pool = arg;
	int nw = pool->nw, rc, cores, core;
	struct work *work = &pool->works[nw];
	int *fd = pool->fd[nw];
	struct task task;
	cpu_set_t cpuset;
	pthread_t id = pthread_self();

	cores = pool->cores;
	core = nw % cores;

	CPU_ZERO(&cpuset);
	CPU_SET(core, &cpuset);
	rc = pthread_setaffinity_np(id, sizeof(cpu_set_t), &cpuset);
	if (rc) {
		perror("setaffinity failed");
		return NULL;
	}

	work->run = 1;
	while (work->run) {
		rc = read(fd[0], &task, sizeof(task));
		if (rc < 0) {
			if (errno == EINTR)
				continue;

			perror("pipe read failed");
		}

		if (rc != sizeof(task)) {
			printf("data incomplete:%d\n", rc);
			continue;
		}

		if (task.func)
			task.func(task.arg);

		Tatomic_subf(&work->task_num, 1);
	}

	return NULL;
}
