#include <thirdparty/limine.h>
#include <types.h>
#include <limine/limine-terminal.h>
#include <stdio.h>
#include <mm/pmm.h>

volatile struct limine_terminal_request terminal_request = {
	.id = LIMINE_TERMINAL_REQUEST,
	.revision = 0,
};

volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
};

static void done(void) {
	for (; ; )
		__asm__ volatile("hlt");
}

void _start(void) {
	if (terminal_request.response == NULL ||
		terminal_request.response->terminal_count < 1) {
			done();
	}

	init_limine_terminal();
	pmm_init();

	done();
}