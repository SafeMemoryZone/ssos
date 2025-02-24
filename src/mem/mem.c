#include "mem.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "misc.h"
#include "paging.h"

#define KERNEL_MEM_ALIGNMENT 16
#define MEM_BLOCKS_MAX_LEVEL_COUNT 32

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

typedef struct mem_block {
	struct mem_block *next;
	uint8_t level;
	bool is_free;
} mem_block_t;

// Level 0 is 1 byte granularity
// Level 31 is 2 GiB granularity
static mem_block_t *mem_blocks[MEM_BLOCKS_MAX_LEVEL_COUNT] = {0};
static size_t highest_block_size = 0x1000;
static int highest_level = 12;

int init_alloc(void) {
	mem_block_t *init_block = alloc_pages(1);

	if (!init_block) {
		return 1;
	}

	mem_blocks[12] = init_block;
	init_block->next = NULL;
	init_block->level = 12;
	init_block->is_free = true;

	return 0;
}

void *kmalloc(size_t bytes) {
	size_t meta_size = ALIGN_UP(sizeof(mem_block_t), KERNEL_MEM_ALIGNMENT);
	bytes += meta_size;  // Extra space for metadata

	if (bytes > highest_block_size) {
		if (highest_level + 1 > MEM_BLOCKS_MAX_LEVEL_COUNT) {
			return NULL;
		}

		highest_block_size *= 2;
		highest_level++;
		mem_blocks[highest_level] = alloc_pages(highest_block_size / 0x1000);

		if (!mem_blocks[highest_level]) {
			return NULL;
		}

		mem_blocks[highest_level]->next = NULL;
		mem_blocks[highest_level]->level = highest_level;
		mem_blocks[highest_level]->is_free = true;
	}

	// Try to find an appropriate block
	int best_level = log2(round_pow2(bytes));

	if (mem_blocks[best_level]) {
		for (mem_block_t *curr_free_block = mem_blocks[best_level]; curr_free_block != NULL;
		     curr_free_block = curr_free_block->next) {
			if (curr_free_block->is_free) {
				curr_free_block->is_free = false;
				return (uint8_t *)curr_free_block + meta_size;
			}
		}
	}

	// Couldn't find a matching block: generate buddies recursively
	size_t required_level = log2(round_pow2(bytes));

	for (int curr_level = highest_level; curr_level >= (int)required_level; curr_level--) {
		// Take a free page from the current level and split it in half

		for (mem_block_t *curr_free_block = mem_blocks[curr_level]; curr_free_block != NULL;
		     curr_free_block = curr_free_block->next) {
			if (curr_free_block->is_free) {
				if ((size_t)curr_level == required_level) {
					curr_free_block->is_free = false;
					return (uint8_t *)curr_free_block + meta_size;
				}

				size_t block_size = 1 << curr_level;

				mem_block_t *block1 = curr_free_block;
				mem_block_t *block2 = (mem_block_t *)((uint8_t *)curr_free_block + block_size / 2);

				block1->next = block2;
				block1->level = curr_level - 1;

				block2->next = mem_blocks[curr_level - 1];
				block2->level = curr_level - 1;
				block2->is_free = true;

				mem_blocks[curr_level - 1] = block1;
			}
		}
	}

	return NULL;
}

void kfree(void *mem) {
	size_t meta_size = ALIGN_UP(sizeof(mem_block_t), KERNEL_MEM_ALIGNMENT);
	mem_block_t *block = (mem_block_t *)((uint8_t *)mem - meta_size);
	block->is_free = true;

	while (block->level < highest_level) {
		size_t block_size = 1 << block->level;
		mem_block_t *buddy = (mem_block_t *)((uintptr_t)block ^ block_size);

		if (!(buddy->is_free && buddy->level == block->level)) {
			break;
		}
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
		if (buddy < block) {
			block = buddy;
		}
		block->level++;
	}

	block->next = mem_blocks[block->level];
	mem_blocks[block->level] = block;
}
