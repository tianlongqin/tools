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
 * Function:  tools share memory.
 * Created on: 2019-08-19
 */
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdint.h>

static int _shm_open(key_t key);

int Tshm_open(const char *pathname, int proj_id, size_t size)
{
	int shmid, rc;
	key_t key = ftok(pathname, proj_id);

	shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
	if (shmid == -1 && errno != EEXIST)
		goto err;

	if (shmid != -1)
		return shmid;

	if(errno == EEXIST)
		shmid = _shm_open(key);
	else
		goto err;

	return shmid;
err:
	return -1;
}

static int _shm_open(key_t key)
{
	int shmid;

	shmid = shmget(key, 0, 0);
	if (shmid == -1)
		goto err;

	return shmid;
err:
	return -1;
}

void *Tshm_attach_ptr(int shmid)
{
	void *ptr = shmat(shmid, NULL, 0);
	if (ptr == (void *)-1)
		return NULL;

	return ptr;
}

int Tshm_attach_num(int shmid)
{
	int rc;
	struct shmid_ds buf;
	rc = shmctl(shmid, IPC_STAT, &buf);
	if (rc == -1)
		return -1;

	return buf.shm_nattch;
}

int Tshm_destroy(int shmid)
{
	return shmctl(shmid, IPC_RMID, 0);
}
