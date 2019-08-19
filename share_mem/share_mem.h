#ifndef __SHARE_MEM_19910612
#define __SHARE_MEM_19910612

#ifdef __cplusplus
extern "C" {
#endif

extern int shm_open(const char *pathname, int proj_id, size_t size);
extern void *shm_attach_ptr(int shmid);
extern int shm_attach_num(int shmid);
extern int shm_destroy(int shmid);

#ifdef __cplusplus
}
#endif
#endif
