#include <mm/pmm.h>
#include <trout/logger.h>
#include <main.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

static u64 highest_addr;
static size_t last_index;
static u8 *bitmap;

static size_t freeram;
static size_t usedram;

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

static bool pmm_bitmap_set(size_t index, bool value)
{
	u64 byteI = index / 8;
	u8 bitI = index % 8;
	u8 bit_indexer = 0x80 >> bitI;

	bool old_val = bitmap[byteI] & bit_indexer;

	bitmap[byteI] &= ~bit_indexer;
	if (value)
		bitmap[byteI] |= bit_indexer;

	return old_val;
}

static bool pmm_bitmap_get(size_t index)
{
	u64 byteI = index / 8;
	u8 bitI = index % 8;
	u8 bit_indexer = 0x80 >> bitI;

	if (bitmap[byteI] & bit_indexer)
		return true;

	return false;
}

static void *pmm_inner_alloc(size_t count, size_t limit)
{
	size_t i = 0;

	while (last_index < limit) {
		if (!pmm_bitmap_get(last_index++)) {
			if (++i == count) {
				size_t page = last_index - count;
				for (size_t j = page; j < last_index; j++)
					pmm_bitmap_set(j, true);
				return (void *)(page * PMM_BLOCK_SIZE);
			}
		}
		else i = 0;
	}
	return NULL;
}

static void pmm_find_highest_addr(struct limine_memmap_entry **entries, u64 entry_count)
{
	for (size_t i = 0; i < entry_count; i++) {
		if (entries[i]->type != LIMINE_MEMMAP_USABLE)
			continue;

		uintptr_t top = entries[i]->base + entries[i]->length;

		if (top > highest_addr)
			highest_addr = top;
	}
}

static void pmm_count_free_memory(struct limine_memmap_entry **entries, u64 entry_count)
{
	for (size_t i = 0; i < entry_count; i++) {
		if (entries[i]->type != LIMINE_MEMMAP_USABLE)
			continue;

		freeram += entries[i]->length;
	}
}

static void pmm_handle_bitmap(struct limine_memmap_entry **entries, u64 entry_count)
{
	size_t bitmap_size = ALIGN_UP((highest_addr / PMM_BLOCK_SIZE) / 8, PMM_BLOCK_SIZE);

	for (size_t i = 0; i < entry_count; i++) {
		if (entries[i]->type != LIMINE_MEMMAP_USABLE)
			continue;

		if (entries[i]->length >= bitmap_size) {
			bitmap = (u8 *)entries[i]->base + hhdm_request.response->offset;
			memset(bitmap, 0xff, bitmap_size);
			entries[i]->length -= bitmap_size;
			entries[i]->base += bitmap_size;
			freeram -= bitmap_size;
			return;
		}
	}

	/* TODO: Proper panic function */
	kinfo("PANIC: The PMM could not find suitable location for the bitmap.");
	for (; ; )
		__asm__("hlt");
}

static void pmm_free_regions(struct limine_memmap_entry **entries, u64 entry_count)
{
	for (size_t i = 0; i < entry_count; i++) {
		if (entries[i]->type != LIMINE_MEMMAP_USABLE)
			continue;

		for (uintptr_t j = 0; j < entries[i]->length; j += PMM_BLOCK_SIZE)
			pmm_bitmap_set((entries[i]->base + j) / PMM_BLOCK_SIZE, false);
	}
}

void *pmm_alloc_blocks(size_t count)
{
	size_t i = last_index;

	void *ret = pmm_inner_alloc(count, highest_addr / PMM_BLOCK_SIZE);

	if (ret == NULL) {
		last_index = 0;
		ret = pmm_inner_alloc(count, i);
		if (ret == NULL) {
			kinfo("PANIC: The PMM is out of memory to work with.");
			for (; ; )
				__asm__("hlt");
		}
	}

	memset((void *)((u64)ret + hhdm_request.response->offset), 0, count * PMM_BLOCK_SIZE);

	usedram += count * PMM_BLOCK_SIZE;
	freeram -= count * PMM_BLOCK_SIZE;

	return ret;
}

void pmm_free_blocks(void *ptr, size_t count)
{
	if (ptr == NULL)
		return;

	size_t page = (size_t)ptr / PMM_BLOCK_SIZE;

	for (size_t i = page; i < page + count; i++)
		pmm_bitmap_set(i, false);

	if (last_index > page)
		last_index = page;

	usedram -= count * PMM_BLOCK_SIZE;
	freeram += count * PMM_BLOCK_SIZE;
}

void pmm_init(void)
{
	kinfo("Initializing PMM...");

	struct limine_memmap_entry **entries = memmap_request.response->entries;
	u64 entry_count = memmap_request.response->entry_count;

	pmm_dump_memmap(entries, entry_count);

	pmm_find_highest_addr(entries, entry_count);

	pmm_count_free_memory(entries, entry_count);

	pmm_handle_bitmap(entries, entry_count);

	pmm_free_regions(entries, entry_count);

	kinfo("Bitmap is stored at virtual address: %p", bitmap);

	kinfo("Free Memory: %dMB", freeram / 1024 / 1024);

	kinfo("PMM Initialized.");
}