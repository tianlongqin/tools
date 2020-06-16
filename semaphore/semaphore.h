#ifndef __SEMAPHORE__HH__19910612__
#define __SEMAPHORE__HH__19910612__

#ifdef __cplusplus
extern "C" {
#endif

extern int Tsem_create(const char *pathname, int proj_id);
extern void Tsem_destroy(int semid);
extern int Tsem_lock(int semid);
extern int Tsem_unlock(int semid);

#ifdef __cplusplus
}
#endif

#endif
