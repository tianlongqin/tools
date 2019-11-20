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
 * Function:  tools file.
 * Created on: 2019-08-19
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>


/*
 * Tfile_check_type_fd - check file type 
 * @fd: file descriptor 
 *
 * On success, type is returned.  On error, -1 is returned, and errno is set appropriately.
 */
int Tfile_check_type_fd(int fd, int type)
{
	struct stat stat;

	if (fstat(fd, &stat) < 0) {
		perror("fstat error:");
		return -1;
	}

	return stat.st_mode & type;
}

/*
 * Tfile_check_type_name - check file type 
 * @fd: file descriptor 
 *
 * On success, type is returned.  On error, -1 is returned, and errno is set appropriately.
 */
int Tfile_check_type_name(const char *name, int type)
{
	struct stat st;

	if (stat(name, &st) < 0) {
		perror("fstat error:");
		return -1;
	}

	return st.st_mode & type;
}

/*
 * Tfile_get_size_fd - get file tatol size 
 * @fd: file descriptor 
 *
 * On success, size is returned.  On error, -1 is returned, and errno is set appropriately.
 */
int Tfile_get_size_fd(int fd)
{
	struct stat stat;

	if (fstat(fd, &stat) < 0) {
		perror("fstat error:");
		return -1;
	}
	
	return stat.st_size;
}

/*
 * Tfile_get_size_name - get file tatol size 
 * @name: file name
 *
 * On success, size is returned.  On error, -1 is returned, and errno is set appropriately.
 */
int Tfile_get_size_name(const char *name)
{
	struct stat st;
	if (stat(name, &st) < 0) {
		perror("fstat error:");
		return -1;
	}

	return st.st_size;
}
