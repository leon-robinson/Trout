#include <trout/logger.h>
#include <stdio.h>
#include <stdarg.h>

void kinfo_no_newline(const char *fmt, ...)
{
	printf("[INFO] ");
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

void kinfo(const char *fmt, ...)
{
	printf("[INFO] ");
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	putchar('\n');
}