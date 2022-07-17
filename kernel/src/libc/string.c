#include <string.h>
#include <types.h>

size_t strlen(const char *str)
{
	size_t len = 0;
	while (*str++)
		len++;
	return len;
}

void *memset(void *str, int c, size_t n)
{

	void *d = str;

#define DO_SET(T, sz, v) \
		do { \
			size_t x = (size_t)sz; \
			while (x >= sizeof(T)) { \
				*(T*)d = (T)v; \
				x -= sizeof(T); \
				d = (void *)((char *)(d) + sizeof(T)); \
				n -= sizeof(T);  \
			} \
		} while(0)

	if (n >= 4 && ((uintptr_t)str & 3))
		DO_SET(u8, n & 3, c);

	DO_SET(u32, n, ((u32)c << 24 | (u32)c << 16 | (u32)c << 8 | (u32)c));

	DO_SET(u8, n, c);

#undef DO_SET
	return str;
}

void *memcpy(void *dest, const void * src, size_t n)
{
#define DO_COPY(T, sz) \
		do { \
			size_t x = (size_t)sz; \
			while (x >= sizeof(T)) { \
				*(T*)d = *(T*)s; \
				x -= sizeof(T); \
				s += sizeof(T); \
				d += sizeof(T); \
				n -= sizeof(T);  \
			} \
		} while(0)

	char *d = (char *)dest;
	const char *s = (const char *)src;

	if (n >= 4 && ((uintptr_t)dest & 3))
		DO_COPY(u8, n & 3);

	DO_COPY(u32, n);

	DO_COPY(u8, n);

#undef DO_COPY
	return dest;
}