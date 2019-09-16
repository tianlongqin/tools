#ifndef __SIGNAL_HH_19910612__
#define __SIGNAL_HH_19910612__

#ifdef __cplusplus
extern "C" {
#endif

extern int block_sig(int sig);
extern int unblock_sig(int sig);
extern int block_all_sig(void);
extern int unblock_all_sig(void);

#ifdef __cplusplus
}
#endif

#endif
