#ifndef __THREAD_POOL__h__
#define __THREAD_POOL__h__

#define TPOOL_LONG 0
#define TPOOL_SHORT 1

#ifdef __cplusplus
extern "C" {
#endif
	int Tthreadpool_create(void **__pool, unsigned int num);

	void Tthreadpool_destroy(void **_pool);

	void Tthreadpool_loop(void *_pool);

	int Tthreadpool_add_task(void *_pool, int core_id, void *(*function)(void *arg), void *arg);

	int Tthreadpool_get_minid(void *_pool);
#ifdef __cplusplus
}
#endif

#endif
