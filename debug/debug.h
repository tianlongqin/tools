#ifndef __DEBUG__H_19910612
#define __DEBUG__H_19910612

#ifdef CONFIG_DEBUG
#define Tprint_debug			printf
#define Tprint_hex_dump_debug		Tprint_hex_dump
#define Tprint_err_debug(fmt, ...)	Tprint_err(__LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define Tprint_info_debug(fmt, ...)	Tprint_info(__LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

extern void Tprint_hex_dump(const char *prompt, const void *buffer, size_t size);
extern void Tprint_err(int line, const char *func, const char *szFormat, ...);
extern void Tprint_info(int line, const char *func, const char *szFormat, ...);

#ifdef __cplusplus
}
#endif

#else /* CONFIG_DEBUG */
#define Tprint_debug
#define Tprint_hex_dump_debug
#define Tprint_err_debug(fmt, ...)
#define Tprint_info_debug(fmt, ...)
#endif


#endif
