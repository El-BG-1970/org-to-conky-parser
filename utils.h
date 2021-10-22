#include <stddef.h>

char *rstrchr(const char *s, char *start, char c) {
	char *ret = start;
	while ((ret > s) && (*ret != c)) ret--;
	if ((s == ret) && (*s != c)) return NULL;
	else return ret;
}
