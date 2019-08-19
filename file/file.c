#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>

int file_b_fifo(int fd, int type)
{
	struct stat stat;

	if (fstat(fd, &stat) < 0) {
		perror("fstat error:");
		return -1;
	}

	return stat.st_mode & type;
}
