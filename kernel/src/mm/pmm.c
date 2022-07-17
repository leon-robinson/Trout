#include <mm/pmm.h>
#include <trout/logger.h>
#include <main.h>
#include <stdio.h>

//static u64 highest_addr;
//static size_t last_index;
//static u8 *bitmap;

static size_t freeram;
static size_t usedram;

//static u64 memtop;

size_t pmm_free_mem(void)
{
	return freeram;
}

size_t pmm_used_mem(void)
{
	return usedram;
}

static void pmm_dump_memmap_header(void)
{
	kinfo_no_newline("");
	putchar(218);
	printf("Type");
	for (int i = 0; i < 7; i++)
		putchar(196);
	printf("Base");
	for (int i = 0; i < 16; i++)
		putchar(196);
	printf("End");
	for (int i = 0; i < 7; i++)
		putchar(196);
	putchar(191);
	putchar('\n');
}

static void pmm_dump_memmap(struct limine_memmap_entry **entries, u64 entry_count)
{
	kinfo("Physical Memory Map:");

	pmm_dump_memmap_header();

	for (size_t i = 0; i < entry_count; i++)
		kinfo("%c %d   %p - %p %c", 179, entries[i]->type, entries[i]->base, (entries[i]->base + entries[i]->length), 179);

	kinfo_no_newline("%c", 192);

	for (int i = 0; i < 41; i++)
		putchar(196);
	putchar(217);
	putchar('\n');
}

void pmm_init(void)
{
	kinfo("Initializing PMM...");
	struct limine_memmap_entry **entries = memmap_request.response->entries;
	u64 entry_count = memmap_request.response->entry_count;

	pmm_dump_memmap(entries, entry_count);

	kinfo("PMM Initialized.");
}