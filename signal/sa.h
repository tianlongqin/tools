#ifndef __SIGNAL_HH_19910612__
#define __SIGNAL_HH_19910612__

#ifdef __cplusplus
extern "C" {
#endif

extern int sig_block(int sig);
extern int sig_unblock(int sig);
extern int sig_block_all(void);
extern int sig_unblock_all(void);
extern int sig_ignore(int sig);

#ifdef __cplusplus
}
#endif

#endif
