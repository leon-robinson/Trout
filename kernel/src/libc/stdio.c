#include <stdio.h>
#include <limine/limine-terminal.h>

int puts(const char *str)
{
	puts_limine_terminal(str);
	putch_limine_terminal('\n');
	return 0;
}