#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#ifdef _SEM_SEMUN_UNDEFINED
union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
				    (Linux-specific) */
};
#endif

static struct sembuf const up = {0, 1, SEM_UNDO};
static struct sembuf const down = {0, -1, SEM_UNDO};
static int _sem_open(key_t key);

int sem_open(const char *pathname, int proj_id)
{
	int semid, rc;
	union semun arg;
	struct sembuf sembuf;
	key_t key = ftok(pathname, proj_id);

	semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if(semid == -1) {
		semid = _sem_open(key);
		if (semid == -1)
			goto __exit;
	} else {
		arg.val = 0;
		rc = semctl(semid, 0, SETVAL, arg);
		if (rc == -1)
			goto __exit;

		sembuf.sem_num = 0;
		sembuf.sem_op = 1;
		sembuf.sem_flg = 0;

		rc = semop(semid, &sembuf, 1);
		if (rc == -1)
			goto __exit;
	}

__exit:
	rc = semid;
	return rc;
}

static int _sem_open(key_t key)
{
	int id, rc, i;
	union semun arg;
	struct semid_ds ds;
	id = semget(key, 1, 0666);
	if(id == -1)
		goto err;

	arg.buf = &ds;

	for (i = 0; i < 10; i++) {
		rc = semctl(id, 0, IPC_STAT, arg);
		if (rc == -1)
			goto err;

		if(ds.sem_otime != 0)
			break;

		usleep(10 * 1000);
	}

	if (ds.sem_otime == 0)
		goto err;

	return id;
err:
	return -1;
}

int sem_lock(int semid)
{
	return semid == -1 ? -1 : semop(semid, (struct sembuf *)&down, 1);
}

int sem_unlock(int semid)
{
	return semid == -1 ? -1 : semop(semid, (struct sembuf *)&up, 1);
}
