#include "paging.h"

#include <stddef.h>
#include <stdint.h>

#include "misc.h"

#define TOTAL_PAGES 2048
#define TOTAL_BUCKET_COUNT (TOTAL_PAGES / 64)

extern void *kernel_start;
extern void *kernel_end;
static uint64_t pages[TOTAL_BUCKET_COUNT] = {0};
static void *page_base;

void init_page_frame_allocator(struct limine_memmap_entry *mmap_entries, size_t entry_count) {
	struct limine_memmap_entry largest_usable_block = {0};
	bool first_block_present = false;

	for (struct limine_memmap_entry *mmap_entry = mmap_entries;
	     mmap_entry < mmap_entries + entry_count; mmap_entry++) {
		if (mmap_entry->type == LIMINE_MEMMAP_USABLE) {
			uintptr_t region_start = ALIGN_UP(mmap_entry->base, 0x1000);
			uintptr_t region_end = ALIGN_DOWN(mmap_entry->base + mmap_entry->length, 0x1000);

			if (region_end <= region_start) {
				continue;
			}

			uintptr_t kstart = ALIGN_UP((uintptr_t)kernel_start, 0x1000);
			uintptr_t kend = ALIGN_DOWN((uintptr_t)kernel_end, 0x1000);

			// Determine the usable subregion
			uintptr_t usable_start = region_start;
			uintptr_t usable_end = region_end;

			if (region_start < kend && region_end > kstart) {
				uint64_t size_before = (kstart > region_start) ? (kstart - region_start) : 0;
				uint64_t size_after = (region_end > kend) ? (region_end - kend) : 0;
				if (size_before >= size_after) {
					usable_end = kstart;
				}
				else {
					usable_start = kend;
				}
			}

			if (usable_end <= usable_start + 0x1000) {
				continue;
			}

			uint64_t usable_length = usable_end - usable_start;

			// Update largest_usable_block if this region is bigger
			if (!first_block_present || usable_length > largest_usable_block.length) {
				largest_usable_block = *mmap_entry;
				largest_usable_block.base = usable_start;
				largest_usable_block.length = usable_length;
				first_block_present = true;
			}
		}
	}

	if (first_block_present) {
		page_base = (void *)largest_usable_block.base;
	}
}

void *alloc_pages(size_t count) {
	uintptr_t bucket_base_addr = (uintptr_t)page_base;
	size_t subsq_count = 0;
	uintptr_t bit_base_addr = bucket_base_addr;

	for (int bucket_idx = 0; bucket_idx < TOTAL_BUCKET_COUNT; bucket_idx++) {
		if (pages[bucket_idx]) {
			for (int bit_idx = 0; bit_idx < 64; bit_idx++) {
				if (pages[bucket_idx] & 1ULL << bit_idx) {
					subsq_count++;
				}
				else {
					subsq_count = 0;
					bit_base_addr = bucket_base_addr + bit_idx * 0x1000;
				}

				if (subsq_count == count) {
					// mark pages as used
					uintptr_t curr_addr = bit_base_addr;

					for (size_t i = 0; i < count; i++) {
						pages[curr_addr / 0x1000 - (uintptr_t)page_base] &= ~(1ULL << (i % 64));
						curr_addr += 0x1000;
					}
					return (void *)bit_base_addr;
				}
			}
		}

		bucket_base_addr += 64 * 0x1000;
	}

	return NULL;
}

void free_pages(void *addr) {
	if ((uintptr_t)addr % 0x1000 != 0) {
		return;
	}

	if (addr < page_base) {
		return;
	}

	int page_off = ((uintptr_t)addr - (uintptr_t)page_base) / 0x1000;

	int bucket_idx = page_off / 0x1000;
	int bit_idx = page_off % 64;

	if (bucket_idx >= TOTAL_BUCKET_COUNT || bit_idx >= 64) {
		return;
	}

	pages[bucket_idx] |= 1ULL << bit_idx;
}
