#ifndef __SEMAPHORE__HH__19910612__
#define __SEMAPHORE__HH__19910612__

extern int sem_open(const char *pathname, int proj_id);
extern int sem_lock(int semid);
extern int sem_unlock(int semid);
#endif
