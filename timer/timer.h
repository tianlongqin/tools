#ifndef __TIMER_HH_19910612__
#define __TIMER_HH_19910612__
#include <time.h>


#define TCLK_EVENT_FLAG_NOT_THREAD	0
#define TCLK_EVENT_FLAG_THREAD		1

struct clk_event {
	struct timespec start;
	struct timespec interval;
	void *(*handle)(void *);
	void *args;
	int flags;
};

#ifdef __cplusplus
extern "C" {
#endif
extern int Tclk_nanosleep(time_t tv_sec, long tv_nsec);
extern int Tclk_init(void);
extern void Tclk_exit(void);
extern int Tclk_new(void **timer, struct clk_event *event);
extern void Tclk_del(void **timer);

#ifdef __cplusplus
}
#endif

#endif
