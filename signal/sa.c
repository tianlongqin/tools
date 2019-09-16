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
 * Function:  tools signal.
 * Created on: 2019-09-16
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define rc_sig(fun) ({ \
	rc = fun; \
	if (rc < 0)  \
		return rc; \
})\

static inline int _block_sig(sigset_t *ss)
{
	return sigprocmask(SIG_BLOCK, ss, NULL);
}

static inline int _unblock_sig(sigset_t *ss)
{
	return sigprocmask(SIG_UNBLOCK, ss, NULL);
}

/*
 * block signal 
 */
int block_sig(int sig)
{
	int rc;
	sigset_t ss;
	rc_sig(sigemptyset(&ss));
	rc_sig(sigaddset(&ss, sig));
	rc_sig(_block_sig(&ss));

	return rc;
}

/*
 * unblock signal
 */
int unblock_sig(int sig)
{
	int rc;
	sigset_t ss;
	rc_sig(sigemptyset(&ss));
	rc_sig(sigaddset(&ss, sig));
	rc_sig(_unblock_sig(&ss));

	return rc;
}

/*
 * block all signal
 */
int block_all_sig(void)
{
	int rc;
	sigset_t ss;

	rc_sig(sigfillset(&ss));
	rc_sig(_block_sig(&ss));

	return rc;
}

/*
 * unblock all signal
 */
int unblock_all_sig(void)
{
	int rc;
	sigset_t ss;

	rc_sig(sigemptyset(&ss));
	rc_sig(_unblock_sig(&ss));

	return rc;
}
