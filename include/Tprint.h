/**
 * @file print.h
 * @brief Helpful print message
 * @author Gao Yuan <gaoyuan@yytek.com>
 *
 * @note borrowed from include/linux/printk.h
 */

#ifndef __PRINT__19910612__
#define __PRINT__19910612__

#include <generated/autoconf.h>

#ifdef CONFIG_ZLOG
#include <zlog.h>
#include <Ttypes.h>

#define ZLOG_CONFIG_STRING				\
	"[global]\n"					\
	"strict init = true\n"				\
	"buffer min = 1024\n"				\
	"buffer max = 2MB\n"				\
	"file perms = 600\n"				\
	"[formats]\n"					\
	"info = \"%m\"\n" 				\
	"debug = \"%d %V->%m\"\n" 			\
	"warn = \"%d %F:%L %V->%m\"\n" 			\
	"error = \"%d %us %V [%p](%F->%U:%L) %m\"\n"	\
	"[rules]\n"					\
	"tools.=info \"/dev/tty\"; info\n" 		\
	"tools.=debug \"/dev/tty\"; debug\n" 		\
	"tools.=warn \"/dev/tty\"; warn\n" 		\
	"tools.=error \"/dev/tty\"; error\n" 		\
	"tools.* \"" CONFIG_LOG_DIR "\", " CONFIG_LOG_FILESIZE " * " CONFIG_LOG_ROTATE " ; \n"

#define CONFIG_LOG_CONFIG_PATH "/etc/tools_zlog.conf"

static int __dzlog_init(void)
{
	int rc;
	FILE *fp;

	rc = dzlog_init(CONFIG_LOG_CONFIG_PATH, "tools");
	if (rc) {
		printf("%s\n", ZLOG_CONFIG_STRING);
		rc = dzlog_init(ZLOG_CONFIG_STRING, "tools");
		if (rc)
			return 1;

		fp = fopen(CONFIG_LOG_CONFIG_PATH, "tools");
		if (!fp)
			/*
			 * We don't have permission of file,
			 * but zlog has been initialized.
			 * */
			return 0;

		fprintf(fp, "%s", ZLOG_CONFIG_STRING);
		fclose(fp);
	}

	return 0;
}

static inline int __zlog_init_check(void)
{
	if (likely(zlog_init_check()))
		return 0;

	return __dzlog_init();
}

#define zlog_check(x)	({ 			\
	if (likely(!__zlog_init_check()))	\
		x;				\
})

#define pr_err(fmt, ...)	zlog_check(dzlog_error(fmt, ##__VA_ARGS__))
#define pr_warning(fmt, ...)	zlog_check(dzlog_warn(fmt, ##__VA_ARGS__))
#define pr_warn			pr_warning
#define pr_info(fmt, ...)	zlog_check(dzlog_info(fmt, ##__VA_ARGS__))

#define pr_debug(fmt, ...)	zlog_check(dzlog_debug(fmt, ##__VA_ARGS__))

#else		/* CONFIG_ZLOG */
#include <stdio.h>

#define pr_err(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#define pr_warning(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#define pr_warn			pr_warning
#define pr_info(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#define pr_debug(fmt, ...)	printf(PR_PFX fmt, ##__VA_ARGS__)

#endif /* CONFIG_DEFAULT */

#include <stdio.h>
#include <string.h>

static __attribute__((unused)) void print_hex_dump(
		const char *prompt, const void *buffer, size_t size)
{
	int i, j, m, n;
	int same;
	const unsigned char *c;
	char buf[256];

	for (i = 0, c = buffer, same = 0; ; i += j) {
		n = 0;
		memset(buf, 0, sizeof(buf));

		if (prompt)
			n = sprintf(buf, "%s", prompt);

		if (i >= size)
			break;

		if (i >= 0x10 && (i + 0x10) <= size) {
			if (!memcmp(c + i - 0x10, c + i, 0x10)) {
				if (!same)
					pr_info("*\n");

				same = 1;
				j = 0x10;
				continue;
			}
		}

		n += sprintf(buf + n, "%08x  ", i);

		same = 0;
		for (j = 0; j < 0x10; j++) {
			m = i + j;
			if (m < size)
				n += sprintf(buf + n, "%02x ", c[m]);
			else
				n += sprintf(buf + n, "   ");

			if (j == 7 || j == 15)
				n += sprintf(buf + n, " ");

		}
		n += sprintf(buf + n, "|");

		for (j = 0; j < 0x10; j++) {
			m = i + j;
			if (m < size)
				n += sprintf(buf + n, "%c", c[m] < 32 || c[m] > 126 ? '.' : c[m]);
			else
				break;
		}

		n += sprintf(buf + n, "|\n");
		pr_info("%s", buf);
	}
	pr_info("%s%08x\n", prompt, i);
}

#endif /* __PRINT__19910612__ */
