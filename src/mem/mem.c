#include "mem.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "misc.h"
#include "paging.h"

#define KERNEL_MEM_ALIGNMENT 16
#define MEM_BLOCKS_MAX_LEVEL_COUNT 34
#define MEM_BLOCKS_BASE_LEVEL 3

typedef struct mem_block {
	struct mem_block *next;
	uint8_t level;
	bool is_free;
} mem_block_t;

static mem_block_t *mem_blocks[MEM_BLOCKS_MAX_LEVEL_COUNT] = {0};
static size_t highest_block_size = PAGE_SIZE;
static int highest_level = LOG2_PAGE_SIZE - MEM_BLOCKS_BASE_LEVEL;

int kalloc_init(void) {
	mem_block_t *init_block = paging_alloc_pages(1);
	if (!init_block) {
		return RET_ERR;
	}
	mem_blocks[highest_level] = init_block;
	init_block->next = NULL;
	init_block->level = highest_level;
	init_block->is_free = true;
	return RET_OK;
}

void *kmalloc(size_t bytes) {
	if (bytes == 0) {
		return NULL;
	}

	size_t meta_size = ALIGN_UP(sizeof(mem_block_t), KERNEL_MEM_ALIGNMENT);
	bytes += meta_size;

	while (bytes > highest_block_size) {
		if (highest_level + 1 >= MEM_BLOCKS_MAX_LEVEL_COUNT) {
			return NULL;
		}

		highest_block_size *= 2;
		highest_level++;

		mem_blocks[highest_level] = paging_alloc_pages(highest_block_size / PAGE_SIZE);

		if (!mem_blocks[highest_level]) {
			return NULL;
		}

		mem_blocks[highest_level]->next = NULL;
		mem_blocks[highest_level]->level = highest_level;
		mem_blocks[highest_level]->is_free = true;
	}

	int target_level = log2(round_pow2(bytes)) - MEM_BLOCKS_BASE_LEVEL;

	if (mem_blocks[target_level]) {
		mem_block_t *free_block = mem_blocks[target_level];
		mem_blocks[target_level] = free_block->next;
		free_block->is_free = false;
		free_block->next = NULL;

		return (uint8_t *)free_block + meta_size;
	}

	// If no free block was found, try splitting larger blocks
	for (int curr_level = highest_level; curr_level >= target_level; curr_level--) {
		mem_block_t *free_block = mem_blocks[curr_level];

		if (!free_block) {
			continue;
		}

		mem_blocks[curr_level] = free_block->next;

		if (curr_level == target_level) {
			free_block->is_free = false;
			free_block->next = NULL;

			return (uint8_t *)free_block + meta_size;
		}

		size_t block_size = 1ULL << (curr_level + MEM_BLOCKS_BASE_LEVEL);

		mem_block_t *block1 = free_block;
		mem_block_t *block2 = (mem_block_t *)((uint8_t *)free_block + block_size / 2);

		block1->level = curr_level - 1;
		block1->is_free = true;
		block1->next = mem_blocks[curr_level - 1];
		mem_blocks[curr_level - 1] = block1;

		block2->level = curr_level - 1;
		block2->is_free = true;
		block2->next = mem_blocks[curr_level - 1];
		mem_blocks[curr_level - 1] = block2;
	}

	return NULL;
}

void kfree(void *mem) {
	if (!mem) {
		return;
	}

	size_t meta_size = ALIGN_UP(sizeof(mem_block_t), KERNEL_MEM_ALIGNMENT);
	mem_block_t *block = (mem_block_t *)((uint8_t *)mem - meta_size);
	block->is_free = true;

	// Attempt to merge buddy blocks
	while (block->level < highest_level) {
		size_t block_size = 1ULL << (block->level + MEM_BLOCKS_BASE_LEVEL);
		mem_block_t *buddy = (mem_block_t *)((uintptr_t)block ^ block_size);

		if (!(buddy->is_free && buddy->level == block->level)) {
			break;
		}
		// Remove the buddy from the free list
		if (mem_blocks[block->level] == buddy) {
			mem_blocks[block->level] = buddy->next;
		}
		else {
			mem_block_t *prev = mem_blocks[block->level];
			while (prev && prev->next != buddy) {
				prev = prev->next;
			}
			if (prev) {
				prev->next = buddy->next;
			}
		}
		// Choose the block with the lower address
		if (buddy < block) {
			block = buddy;
		}
		block->level++;
	}

	// Add the block back to the freelist
	block->next = mem_blocks[block->level];
	mem_blocks[block->level] = block;
}

void *memcpy(void *dest, const void *src, size_t n) {
	uint8_t *pdest = (uint8_t *)dest;
	const uint8_t *psrc = (const uint8_t *)src;
	for (size_t i = 0; i < n; i++) {
		pdest[i] = psrc[i];
	}
	return dest;
}

void *memset(void *s, int c, size_t n) {
	uint8_t *p = (uint8_t *)s;
	for (size_t i = 0; i < n; i++) {
		p[i] = (uint8_t)c;
	}
	return s;
}

void *memmove(void *dest, const void *src, size_t n) {
	uint8_t *pdest = (uint8_t *)dest;
	const uint8_t *psrc = (const uint8_t *)src;
	if (src > dest) {
		for (size_t i = 0; i < n; i++) {
			pdest[i] = psrc[i];
		}
	}
	else if (src < dest) {
		for (size_t i = n; i > 0; i--) {
			pdest[i - 1] = psrc[i - 1];
		}
	}
	return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	const uint8_t *p1 = (const uint8_t *)s1;
	const uint8_t *p2 = (const uint8_t *)s2;
	for (size_t i = 0; i < n; i++) {
		if (p1[i] != p2[i]) {
			return p1[i] < p2[i] ? -1 : 1;
		}
	}
	return 0;
}

size_t strlen(const char *str) {
	size_t len = 0;
	while (*str++) len++;
	return len;
}
