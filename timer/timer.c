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
#include <time.h>
#include <signal.h>

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
