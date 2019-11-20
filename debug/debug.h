#ifndef __DEBUG__H_19910612
#define __DEBUG__H_19910612

#define Tprint_err_debug(fmt, ...)	Tprint_err(__LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define Tprint_info_debug(fmt, ...)	Tprint_info(__LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

extern void Tprint_debug(const char *szFormat, ...);
extern void Tprint_hex_dump_debug(const char *prompt, const void *buffer, size_t size);
extern void Tprint_err(int line, const char *func, const char *szFormat, ...);
extern void Tprint_info(int line, const char *func, const char *szFormat, ...);
extern int Thex_to_bin(char ch);
extern int Thex2bin(unsigned char *dst, const char *src, size_t count);

#ifdef __cplusplus
}
#endif

#endif
