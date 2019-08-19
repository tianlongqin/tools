#ifndef __DEBUG__H_19910612
#define __DEBUG__H_19910612

#ifdef CONFIG_DEBUG
#define print_hex_dump_debug	print_hex_dump
#else
#define print_hex_dump_debug
#endif

#define BIO_W_NULL(r, func) do { \
		if ((r = func) == NULL) \
			printf("func->" #func " line:%d " #r "=%d warning\n", __LINE__, r); \
	} while (0) \

#define BIO_E_NULL(r, func) do { \
		if ((r = func) == NULL) {\
			printf("func->" #func " line:%d " #r "=%d error\n", __LINE__, r); \
			exit(1); \
		} 		\
	} while (0) \


#ifdef __cplusplus
extern "C" {
#endif

extern void print_hex_dump(const char *prompt, const void *buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif
