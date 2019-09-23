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
 * Function:  tools serial port.
 * Created on: 2019-09-02
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#define SERIAL_PFX			"serial: "
#define ARRAY_SIZE(x)           (sizeof(x) / sizeof((x)[0]))

static const int speed_arr[] = {
	B4000000, B2000000, B1152000, B115200, B57600, B38400,
	B19200, B9600, B4800, B2400, B1200, B300,
};
static const int name_arr[] = {
	4000000, 2000000, 1152000, 115200, 57600, 38400,
	19200, 9600, 4800, 2400, 1200, 300,
};


/**
 * @brief open serial device
 * @param dev device name
 * @param block isn't block
 * @return -e serial_open() return the new file descriptor, or -1 on error.
 */
int Tserial_open(char *dev, char block)
{
	int fd, flags;

	flags = O_RDWR | O_NOCTTY;
	if (block & 1)
		flags |= O_NONBLOCK;

	fd = open(dev, flags);
	if (fd < 0)
		printf(SERIAL_PFX "can't open device: %s, errno %d\n",
				dev, errno);

	return fd;
}

/**
 * @brief set i/o speed for serial device
 * @param fd file descriptor
 * @param speed serial speed
 * @return \e serial_set_speed return 0 on success, or -1 on error.
 */
int Tserial_set_speed(int fd, int speed)
{
	int i;
	struct termios options;

	tcgetattr(fd, &options);
	for(i = 0; i < ARRAY_SIZE(speed_arr); i++) {
		if (speed == name_arr[i]) {
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&options, speed_arr[i]);
			cfsetospeed(&options, speed_arr[i]);
			if (tcsetattr(fd, TCSANOW, &options)) {
				printf(SERIAL_PFX "tcsetattr failed, errno %d\n", errno);
				return -1;
			}
			tcflush(fd, TCIOFLUSH);
		}
	}

	return 0;
}

/**
 * @brief setup databits, parity, flow of serial
 * @param fd file descriptor
 * @param databits data bits
 * @param stopbits stop bits
 * @param parity parity mode
 * @return return 0 on success, or -1 on error.
 */
int Tserial_set_parity(int fd, int databits, int stopbits, int parity, int flow)
{
	struct termios options;

	if (tcgetattr(fd, &options)) {
		printf("tcgetattr failed, errno %d\n", errno);
		return -1;
	}

	options.c_cflag &= ~CSIZE;
	switch(databits) {
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
		default:
			options.c_cflag |= CS8;
			break;
	}

	switch(parity) {
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB);
			options.c_iflag |= INPCK;	/* Disnable parity checking */
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;	/* Enable parity */
			options.c_cflag &= ~PARODD;
			options.c_iflag |= INPCK;	/* Disnable parity checking */
			break;
		case 'S':
		case 's':
			options.c_cflag &= ~PARENB;	/* as no parity */
			options.c_cflag &= ~CSTOPB;
			break;
		case 'n':
		case 'N':
		default:
			options.c_cflag &= ~PARENB;	/* Clear parity enable */
			options.c_iflag &= ~INPCK;	/* Enable parity checking */
			break;
	}

	switch(stopbits) {
		case 2:
			options.c_cflag |= CSTOPB;
			break;
		case 1:
		default:
			options.c_cflag &= ~CSTOPB;
			break;
	}

	/* set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;

	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0;

	/* set Raw Mode */
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);	/* Input */
	options.c_oflag &= ~OPOST;				/* Output */

	/* clear Software Flow Control */
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	options.c_iflag &= ~(IXANY);

	options.c_cflag &= ~CRTSCTS;
	switch (flow) {
		case 'd':
			options.c_cflag |= CRTSCTS;
			break;
	}
	options.c_cflag |= CLOCAL | CREAD;

	/* clear 0x0d and 0x0a map */
	options.c_oflag &= ~(ONLCR | OCRNL);
	options.c_iflag &= ~(INLCR | ICRNL);

	tcsetattr(fd, TCSANOW, &options);
	tcflush(fd, TCIFLUSH);

	return 0;
}

int Tserial_open_with_arg(char *dev, char block, int speed, int databits,
		int stopbits, int parity, int flow)
{
	int fd, rc;

	fd = Tserial_open(dev, block);
	if (fd < 0)
		return -1;

	rc = Tserial_set_parity(fd, databits, stopbits, parity, flow);
	if (rc)
		return -1;

	rc = Tserial_set_speed(fd, speed);
	if (rc)
		return -1;

	return fd;
}
