#include <stdio.h>
#include <stdint.h>
#include <string.h>

void print_hex_dump(const char *prompt, const void *buffer, size_t size)
{
	int i, j, m;
	int same;
	const uint8_t *c;

	for (i = 0, c = buffer, same = 0; ; i += j) {
		if (i >= size)
			break;

		if (i >= 0x10 && (i + 0x10) <= size) {
			if (!memcmp(c + i - 0x10, c + i, 0x10)) {
				if (!same)
					printf("*\n");
				same = 1;
				j = 0x10;
				continue;
			}
		}

		if (prompt)
			printf("%s%08x  ", prompt, i);
		else
			printf("%08x  ", i);
		same = 0;
		for (j = 0; j < 0x10; j++) {
			m = i + j;
			if (m < size)
				printf("%02x ", c[m]);
			else
				printf("   ");
			if (j == 7 || j == 15)
				printf(" ");
		}
		printf("|");
		for (j = 0; j < 0x10; j++) {
			m = i + j;
			if (m < size)
				printf("%c", c[m] < 32 || c[m] > 126 ? '.' : c[m]);
			else
				break;
		}
		printf("|\n");
	}
	if (prompt)
		printf("%s%08x\n", prompt, i);
	else
		printf("%08x\n", i);
}
