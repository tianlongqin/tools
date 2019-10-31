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

#define RC_SIG(expression)		\
   (__extension__			\
	({long int __result; 		\
	 __result  = (expression); 	\
	 if (__result < 0)  		\
		return __result;}))	\

#define _sig_block(ss)		sigprocmask(SIG_BLOCK, ss, NULL)
#define _sig_unblock(ss)	sigprocmask(SIG_UNBLOCK, ss, NULL)

/*
 * block signal 
 */
int Tsig_block(int sig)
{
	sigset_t ss;
	RC_SIG(sigemptyset(&ss));
	RC_SIG(sigaddset(&ss, sig));
	RC_SIG(_sig_block(&ss));

	return 0;
}

/*
 * unblock signal
 */
int Tsig_unblock(int sig)
{
	sigset_t ss;
	RC_SIG(sigemptyset(&ss));
	RC_SIG(sigaddset(&ss, sig));
	RC_SIG(_sig_unblock(&ss));

	return 0;
}

/*
 * block all signal
 */
int Tsig_block_all(void)
{
	sigset_t ss;

	RC_SIG(sigfillset(&ss));
	RC_SIG(_sig_block(&ss));

	return 0;
}

/*
 * unblock all signal
 */
int Tsig_unblock_all(void)
{
	sigset_t ss;

	RC_SIG(sigemptyset(&ss));
	RC_SIG(_sig_unblock(&ss));

	return 0;
}

static int _sig_def(int sig, void (*sig_handler) (int))
{
	struct sigaction sa;

	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	RC_SIG(sigemptyset(&sa.sa_mask));

	RC_SIG(sigaction(sig, &sa, NULL));

	return 0;
}

int Tsig_ignore(int sig)
{
	return (_sig_def(sig, SIG_IGN));
}
