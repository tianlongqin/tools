#ifndef __SHARE_MEM_19910612
#define __SHARE_MEM_19910612

#ifdef __cplusplus
extern "C" {
#endif

extern int Tshm_open(const char *pathname, int proj_id, size_t size);
extern void *Tshm_attach_ptr(int shmid);
extern int Tshm_attach_num(int shmid);
extern int Tshm_destroy(int shmid);

#ifdef __cplusplus
}
#endif
#endif
