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

void Tprint_err(int line, const char *func, const char *szFormat, ...)
{
	char pDate[512] = {0};
	struct timeval start;
	gettimeofday(&start, NULL);
	sprintf(pDate, "ERR->[%ld:%ld] [%d-%ld] [%s-%s:(%d)]:",
			start.tv_sec, start.tv_usec, getpid(), pthread_self(), __FILE__, func, line);
	va_list valist;
	va_start(valist, szFormat);
	vsnprintf(pDate + strlen(pDate), 512 - strlen(pDate), szFormat, valist);
	va_end(valist);

	printf("\033[31m%s\033[0m", pDate);
}

void Tprint_info(int line, const char *func, const char *szFormat, ...)
{
	char pDate[512] = {0};
	struct timeval start;
	gettimeofday(&start, NULL);
	sprintf(pDate, "INFO->[%ld:%ld] [%d-%ld] [%s-%s:(%d)]:",
			start.tv_sec, start.tv_usec, getpid(), pthread_self(), __FILE__, func, line);
	va_list valist;
	va_start(valist, szFormat);
	vsnprintf(pDate + strlen(pDate), 512 - strlen(pDate), szFormat, valist);
	va_end(valist);

	printf("%s", pDate);
}

void Tprint_hex_dump(const char *prompt, const void *buffer, size_t size)
{
	int i, j, m;
	int same;
	const uint8_t *c;

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
