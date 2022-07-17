#include <limine/limine-terminal.h>
#include <main.h>
#include <string.h>

static struct limine_terminal *terminal;

void init_limine_terminal(void)
{
	terminal = terminal_request.response->terminals[0];
}

void puts_limine_terminal(const char *str)
{
	terminal_request.response->write(terminal, str, strlen(str));
}

void putch_limine_terminal(char c)
{
	char str[2] = {c,'\0'};
	puts_limine_terminal(str);
}