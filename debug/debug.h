#ifndef __DEBUG__H_19910612
#define __DEBUG__H_19910612

#ifdef CONFIG_DEBUG
#define print_debug			printf
#define print_hex_dump_debug		print_hex_dump
#define print_err_debug(fmt, ...)	print_err(__LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define print_info_debug(fmt, ...)	print_info(__LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

extern void print_hex_dump(const char *prompt, const void *buffer, size_t size);
extern void print_err(int line, const char *func, const char *szFormat, ...);
extern void print_info(int line, const char *func, const char *szFormat, ...);

#ifdef __cplusplus
}
#endif

#else /* CONFIG_DEBUG */
#define print_debug
#define print_hex_dump_debug
#define print_err_debug(fmt, ...)
#define print_info_debug(fmt, ...)
#endif


#endif
