#ifndef __TROUT_PMM_H
#define __TROUT_PMM_H

#include <types.h>

#define PMM_BLOCK_SIZE 64

void pmm_init(void);

size_t pmm_free_mem(void);
size_t pmm_used_mem(void);

void pmm_alloc_blocks(size_t count);
void pmm_free_blocks(void *ptr, size_t count);

#endif