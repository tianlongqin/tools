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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>


#include "file.h"
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

/*
 * Tfile_get_file_count - get file and directory count
 * @path: file path
 * @dcount: directory count
 * @fcount: file count
 * @flags: reserved
 *
 * On success, 0 is returned.  On error, -1 is returned, and errno is set appropriately.
 */
int Tfile_get_file_count(const char *path, uint32_t *dcount, uint32_t *fcount, int flags)
{
	int ret;
	uint32_t _dcount = 0, _fcount = 0;
	DIR *dir;
	struct dirent *ptr;
	struct stat st;
	char dname[PATH_MAX];
	char linkname[PATH_MAX];
	char linkname1[PATH_MAX];

	dir = opendir(path);
	if (!dir) {
		perror("opendir:");
		return -1;
	}

	while ((ptr = readdir(dir)) != NULL) {
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
			continue;

		/* contain hidden file and hidden dir*/
		if(ptr->d_name[0] == '.')
			continue;

		snprintf(dname, sizeof(dname) - 1, "%s/%s", path, ptr->d_name);

		ret = lstat(dname, &st);
		if (ret < 0)
			return -1;

		if (S_ISLNK(st.st_mode)) {
			memset(linkname, 0, PATH_MAX);
			readlink(dname, linkname, PATH_MAX);
			ret = lstat(linkname, &st);
			if (ret < 0) {
				snprintf(linkname1, sizeof(linkname) - 1, "%s/%s", dirname(dname), linkname);
				ret = lstat(linkname1, &st);
			}

			if (S_ISDIR(st.st_mode))
				*dcount += 1;
			else
				*fcount += 1;

			continue;
		}

		if (S_ISDIR(st.st_mode)) {
			Tfile_get_file_count(dname, dcount, fcount, flags);
			*dcount += 1;
		} else {
			*fcount += 1;
		}
	}

	closedir(dir);

	return 0;
}
