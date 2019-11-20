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
 * Function:  tools debug.
 * Created on: 2019-08-19
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <debug.h>
#include <generated/autoconf.h>

enum {
	TDEBUG_LEVEL1,
	TDEBUG_LEVEL2,
	TDEBUG_LEVEL3,
};

#ifdef CONFIG_ENV
#define ENV_DEBUG(l) 					\
	char *level = getenv("TDEBUG");			\
	if (level && (Thex_to_bin(level[0]) > l)) 	\
			return ;
#else
#define ENV_DEBUG(l)
#endif

void Tprint_debug(const char *szFormat, ...)
{
	char pDate[512] = {0};

	ENV_DEBUG(TDEBUG_LEVEL1);

	va_list valist;
	va_start(valist, szFormat);
	vsnprintf(pDate + strlen(pDate), 512 - strlen(pDate), szFormat, valist);
	va_end(valist);

	printf("%s", pDate);
}

void Tprint_info(int line, const char *func, const char *szFormat, ...)
{
	char pDate[512] = {0};
	struct timeval start;

	ENV_DEBUG(TDEBUG_LEVEL2);

	gettimeofday(&start, NULL);
	sprintf(pDate, "INFO->[%ld:%ld] [%d-%ld] [%s-%s:(%d)]:",
			start.tv_sec, start.tv_usec, getpid(), pthread_self(), __FILE__, func, line);
	va_list valist;
	va_start(valist, szFormat);
	vsnprintf(pDate + strlen(pDate), 512 - strlen(pDate), szFormat, valist);
	va_end(valist);

	printf("%s", pDate);
}

void Tprint_err(int line, const char *func, const char *szFormat, ...)
{
	char pDate[512] = {0};
	struct timeval start;

	ENV_DEBUG(TDEBUG_LEVEL3);

	gettimeofday(&start, NULL);
	sprintf(pDate, "ERR->[%ld:%ld] [%d-%ld] [%s-%s:(%d)]:",
			start.tv_sec, start.tv_usec, getpid(), pthread_self(), __FILE__, func, line);
	va_list valist;
	va_start(valist, szFormat);
	vsnprintf(pDate + strlen(pDate), 512 - strlen(pDate), szFormat, valist);
	va_end(valist);

	printf("\033[31m%s\033[0m", pDate);
}

void Tprint_hex_dump(const char *prompt, const void *buffer, size_t size)
{
	int i, j, m;
	int same;
	const uint8_t *c;

	ENV_DEBUG(TDEBUG_LEVEL1);

	for (i = 0, c = buffer, same = 0; ; i += j) {
		if (i >= size)
			break;

		if (i >= 0x10 && (i + 0x10) <= size) {
			if (!memcmp(c + i - 0x10, c + i, 0x10)) {
				if (!same)
					printf("*\n");
				same = 1;
				j = 0x10;
				continue;
			}
		}

		if (prompt)
			printf("%s%08x  ", prompt, i);
		else
			printf("%08x  ", i);
		same = 0;
		for (j = 0; j < 0x10; j++) {
			m = i + j;
			if (m < size)
				printf("%02x ", c[m]);
			else
				printf("   ");
			if (j == 7 || j == 15)
				printf(" ");
		}
		printf("|");
		for (j = 0; j < 0x10; j++) {
			m = i + j;
			if (m < size)
				printf("%c", c[m] < 32 || c[m] > 126 ? '.' : c[m]);
			else
				break;
		}
		printf("|\n");
	}
	if (prompt)
		printf("%s%08x\n", prompt, i);
	else
		printf("%08x\n", i);
}

/**
 * Thex_to_bin - convert a hex digit to its real value
 * @ch: ascii character represents hex digit
 *
 * Thex_to_bin() converts one hex digit to its actual value or -1 in case of bad
 * input.
 **/
int Thex_to_bin(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - '0';
	ch = tolower(ch);
	if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	return -1;
}

/**
 * Thex2bin - convert an ascii hexadecimal string to its binary representation
 * @dst: binary result
 * @src: ascii hexadecimal string
 * @count: result length
 *
 * Return 0 on success, -EINVAL in case of bad input.
 */
int Thex2bin(unsigned char *dst, const char *src, size_t count)
{
	while (count--) {
		int hi = Thex_to_bin(*src++);
		int lo = Thex_to_bin(*src++);

		if ((hi < 0) || (lo < 0))
			return -EINVAL;

		*dst++ = (hi << 4) | lo;
	}
	return 0;
}
