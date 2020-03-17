/*
 * This file is tools member.
 *
 * Copyright (c) 2015-2019, tianlongqin, <qtl_linux@163.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function:  tools timer.
 * Created on: 2019-09-10
 */

#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdlib.h>

#include <list.h>
#include <timer.h>

struct clk_timer {
	struct list_head node;
	char name[16];
	int timerfd;
	struct clk_event event;
};

static struct clk_head {
#define CLK_EVENT_MAX 20
	int epollfd;
	int init;
	int event_run;
	pthread_t eventid;
	struct epoll_event events[CLK_EVENT_MAX];
	struct list_head head;
} clk_head;

static int clk_new(void *timer, struct clk_event *event);
static void clk_del(void *timer);
static int clk_add_event(int timerfd);
static int clk_del_event(int timerfd);
static void *clk_event(void *args);
static void *clk_run_event(int fd);

/*
 * time_t tv_sec // seconds
 * long  tv_nsec // nanoseconds
 */

int Tclk_nanosleep(time_t tv_sec, long tv_nsec)
{
	int rc;
	struct timespec time;

	rc = clock_gettime(CLOCK_MONOTONIC, &time);
	if (rc < 0)
		return rc;

	time.tv_sec += tv_sec;
	time.tv_nsec += tv_nsec;

	do {
		rc = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &time, NULL);
		if (rc <  0) {
			if (errno == EINTR)
				continue;
			else
				break;
		}

	} while (rc != 0);

	return rc;
}

static int clk_new(void *timer, struct clk_event *event)
{
	int ret;
	struct clk_timer *t = (struct clk_timer *)timer;
	struct clk_event *e = &t->event;
	struct itimerspec new_value;

	e->handle = event->handle;
	e->args = event->args;

	new_value.it_value.tv_sec = event->start.tv_sec; //第一次到期的时间
	new_value.it_value.tv_nsec = event->start.tv_nsec;

	new_value.it_interval.tv_sec = event->interval.tv_sec;      //之后每次到期的时间间隔
	new_value.it_interval.tv_nsec = event->interval.tv_nsec;

	t->timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK); // 构建了一个定时器
	if (t->timerfd < 0) {
		printf("timerfd create failed\n");
		return -1;
	}

	ret = timerfd_settime(t->timerfd, 0, &new_value, NULL);//启动定时器

	return clk_add_event(t->timerfd);
}

static void clk_del(void *timer)
{
	struct clk_timer *t = (struct clk_timer *)timer;

	clk_del_event(t->timerfd);
	close(t->timerfd);
}

static int clk_add_event(int timerfd)
{
	struct epoll_event ev;
	ev.data.fd = timerfd;
	ev.events = EPOLLIN | EPOLLET;

	return epoll_ctl(clk_head.epollfd, EPOLL_CTL_ADD, timerfd, &ev);
}

static inline int clk_del_event(int timerfd)
{
	return epoll_ctl(clk_head.epollfd, EPOLL_CTL_DEL, timerfd, NULL);
}

static void *clk_run_event(int fd)
{
	uint64_t exp = 0;
	ssize_t s;
	struct clk_head *head = &clk_head;
	struct clk_timer *t;
	struct clk_event *event;
	/* find timer fd */
	list_for_each_entry(t, &head->head, node) {
		if (fd == t->timerfd) {
			event = &t->event;
			/* 需要读出uint64_t大小, 不然会发生错误 */
			s = read(fd, &exp, sizeof(uint64_t));
			if (s != sizeof(uint64_t))
				perror("clk event read failed:");
			/* run clk timer event function */
			return event->handle(event->args);
		}
	}

	return NULL;
}

static void *clk_event(void *args)
{
	int num, i;
	struct clk_head *head = &clk_head;
	while (head->event_run) {
		num = epoll_wait(head->epollfd, head->events, CLK_EVENT_MAX, -1);
		if (num == -1) {
			if (errno == EINTR) {
				continue;
			} else {
				perror("epoll_wait failed\n");
				exit(1);
			}
		}

		for (i = 0; i < num; i++) {
			if (head->events[i].events & EPOLLIN)
				clk_run_event(head->events[i].data.fd);
		}
	}
}

int Tclk_init(void)
{
	INIT_LIST_HEAD(&clk_head.head);

	clk_head.epollfd = epoll_create(CLK_EVENT_MAX);
	if (clk_head.epollfd < 0) {
		perror("epoll_create failed:");
		return -1;
	}

	clk_head.event_run = 1;
	clk_head.init = 1;

	pthread_create(&clk_head.eventid, NULL, clk_event, NULL);
}

void Tclk_exit(void)
{
	if (!clk_head.init)
		return ;


	clk_head.event_run = 0;
	close(clk_head.epollfd);


	pthread_cancel(clk_head.eventid);
	pthread_join(clk_head.eventid, NULL);
	clk_head.init = 0;
}

int Tclk_new(void **timer, struct clk_event *event)
{
	int rc;
	if (!clk_head.init)
		return -1;

	struct clk_timer *t = calloc(1, sizeof(struct clk_timer));
	if (!t) {
		printf("clk_new calloc failed\n");
		return -1;
	}

	//list_add_tail(&(clk_head.head), &t->node);
	list_add_tail(&t->node, &(clk_head.head));

	rc = clk_new(t, event);
	if (rc < 0) {
		printf("clk create failed\n");
		goto err;
	}

	*timer = (void *)t;
	return 0;
err:
	list_del(&t->node);
	free(t);

	return -1;
}

void Tclk_del(void **timer)
{
	if (!clk_head.init)
		return ;

	struct clk_timer *t = (struct clk_timer *)(*timer);
	if (!t)
		return ;

	clk_del(t);

	list_del(&t->node);
	free(t);
	*timer = NULL;
}
