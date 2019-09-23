#ifndef __SERIAL_FUNC__H_19910612__
#define __SERIAL_FUNC__H_19910612__

#ifdef __cplusplus
extern "C" {
#endif

extern int Tserial_open(char *dev, char block);
extern int Tserial_set_speed(int fd, int speed);
extern int Tserial_set_parity(int fd, int databits, int stopbits, int parity, int flow);
extern int Tserial_open_with_arg(char *dev, char block, int speed, int databits,
		int stopbits, int parity, int flow);

#ifdef __cplusplus
}
#endif

#endif
