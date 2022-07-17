#ifndef __TROUT_STRING_H
#define __TROUT_STRING_H

#include <types.h>

size_t strlen(const char *str);
void *memset(void *str, int c, size_t n);
void *memcpy(void *dest, const void * src, size_t n);

#endif