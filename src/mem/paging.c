#include "paging.h"

#include <stddef.h>
#include <stdint.h>

#include "mem.h"
#include "misc.h"
#include "paging.h"

#define TOTAL_PAGES 2048
#define TOTAL_BUCKET_COUNT (TOTAL_PAGES / 64)
#define BUCKET_SIZE 64

#define PAGE_PRESENT (1ULL << 0)
#define PAGE_RW (1ULL << 1)
#define PAGE_USER (1ULL << 2)
#define PAGE_PWT (1ULL << 3)
#define PAGE_ACCESSED (1ULL << 5)
#define PAGE_DIRTY (1ULL << 6)
#define PAGE_GLOBAL (1ULL << 8)
#define PAGE_EXECUTE_DISABLE (1ULL << 63)
#define PAGE_ADDR_MASK 0x000FFFFFFFFFF000ULL
#define PAGE_ADDR_SHIFT 12

#define RESERVED_SPACE_SIZE 2097152

extern uintptr_t virt_kernel_start;
extern uintptr_t virt_kernel_end;

static uintptr_t hhdm_start;
static uint64_t pages[TOTAL_BUCKET_COUNT] = {0};
static uintptr_t phys_page_base;
static uint64_t *kernel_pml4;

__attribute__((aligned(0x1000))) static uint8_t page_tables[RESERVED_SPACE_SIZE];
void *curr_free_page_table = page_tables;

uint64_t read_cr3(void) {
	uint64_t cr3;
	__asm__ __volatile__("mov %%cr3, %0" : "=r"(cr3));
	return cr3;
}

int init_page_frame_allocator(struct limine_memmap_entry *mmap_entries, size_t entry_count,
                              uint64_t hh_off) {
	kernel_pml4 = (uint64_t *)read_cr3();
	hhdm_start = hh_off;

	uint64_t best_base = 0;
	uint64_t best_len = 0;
	bool first_block_present = false;

	for (struct limine_memmap_entry *mmap_entry = mmap_entries;
	     mmap_entry < mmap_entries + entry_count; mmap_entry++) {
		if (mmap_entry->type == LIMINE_MEMMAP_USABLE) {
			uintptr_t region_start = ALIGN_UP(mmap_entry->base, PAGE_SIZE);
			uintptr_t region_end = ALIGN_DOWN(mmap_entry->base + mmap_entry->length, PAGE_SIZE);

			if (region_end <= region_start) {
				continue;
			}

			uintptr_t kstart = ALIGN_UP(virt_kernel_start - hhdm_start, PAGE_SIZE);
			uintptr_t kend = ALIGN_DOWN(virt_kernel_end - hhdm_start, PAGE_SIZE);

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

			if (usable_end <= usable_start + PAGE_SIZE) {
				continue;
			}

			uint64_t usable_length = usable_end - usable_start;

			// Update largest_usable_block if this region is bigger
			if (!first_block_present || usable_length > best_len) {
				best_base = usable_start;
				best_len = usable_length;
				first_block_present = true;
			}
		}
	}

	if (!first_block_present) {
		return RET_ERR;
	}

	size_t page_count = best_len / PAGE_SIZE;

	for (size_t page_idx = 0; page_idx < (page_count > TOTAL_PAGES ? TOTAL_PAGES : page_count);
	     page_idx++) {
		pages[page_idx / BUCKET_SIZE] |= 1 << (page_idx % BUCKET_SIZE);
	}

	phys_page_base = best_base;
	return RET_OK;
}

uint64_t get_page_entry_with_alloc(bool present, bool rw, bool user, bool pwt, bool accessed,
                                   bool dirty, bool global, bool execute_disable) {
	uint64_t page_entry = 0;
	page_entry |= present ? PAGE_PRESENT : 0;
	page_entry |= rw ? PAGE_RW : 0;
	page_entry |= user ? PAGE_USER : 0;
	page_entry |= pwt ? PAGE_PWT : 0;
	page_entry |= accessed ? PAGE_ACCESSED : 0;
	page_entry |= dirty ? PAGE_DIRTY : 0;
	page_entry |= global ? PAGE_GLOBAL : 0;
	page_entry |= execute_disable ? PAGE_EXECUTE_DISABLE : 0;

	void *virt_page_table_addr = curr_free_page_table;
	memset(virt_page_table_addr, 0, 0x1000);
	curr_free_page_table = (void *)((uintptr_t)curr_free_page_table + 0x1000);
	uintptr_t phys_page_table_addr = (uintptr_t)virt_page_table_addr - hhdm_start;

	page_entry |= phys_page_table_addr << PAGE_ADDR_SHIFT;

	return page_entry;
}

void map_page(void *phys_page_addr, uint64_t *pml4_addr) {
	uintptr_t virt_page_addr = (uintptr_t)phys_page_addr + hhdm_start;

	uintptr_t pml4_idx = (virt_page_addr >> 39) & 0x1FF;
	uintptr_t pdpt_idx = (virt_page_addr >> 30) & 0x1FF;
	uintptr_t pd_idx = (virt_page_addr >> 21) & 0x1FF;
	uintptr_t pt_idx = (virt_page_addr >> 12) & 0x1FF;

	if (!(pml4_addr[pml4_idx] & PAGE_PRESENT)) {
		pml4_addr[pml4_idx] =
		    get_page_entry_with_alloc(true, true, false, false, false, false, false, false);
	}

	uint64_t *pdpt = (uint64_t *)((pml4_addr[pml4_idx] & PAGE_ADDR_MASK) | hhdm_start);
	if (!(pdpt[pdpt_idx] & PAGE_PRESENT)) {
		pdpt[pdpt_idx] =
		    get_page_entry_with_alloc(true, true, false, false, false, false, false, false);
	}

	uint64_t *pd = (uint64_t *)((pdpt[pdpt_idx] & PAGE_ADDR_MASK) | hhdm_start);
	if (!(pd[pd_idx] & PAGE_PRESENT)) {
		pd[pd_idx] =
		    get_page_entry_with_alloc(true, true, false, false, false, false, false, false);
	}

	uint64_t *pt = (uint64_t *)((pd[pd_idx] & PAGE_ADDR_MASK) | hhdm_start);
	pt[pt_idx] = (((uintptr_t)phys_page_addr) & PAGE_ADDR_MASK) | PAGE_PRESENT | PAGE_RW;
}

void *alloc_pages(size_t count) {
	uintptr_t bucket_base_addr = phys_page_base;
	size_t subsq_count = 0;
	uintptr_t bit_base_addr = bucket_base_addr;

	for (int bucket_idx = 0; bucket_idx < TOTAL_BUCKET_COUNT; bucket_idx++) {
		if (pages[bucket_idx]) {
			for (int bit_idx = 0; bit_idx < BUCKET_SIZE; bit_idx++) {
				if (pages[bucket_idx] & 1ULL << bit_idx) {
					subsq_count++;
				}
				else {
					subsq_count = 0;
					bit_base_addr = bucket_base_addr + bit_idx * PAGE_SIZE;
				}

				if (subsq_count == count) {
					// Mark pages as used
					uintptr_t curr_addr = bit_base_addr;

					for (size_t i = 0; i < count; i++) {
						map_page((void *)curr_addr, kernel_pml4);
						size_t page_idx = (curr_addr - phys_page_base) / PAGE_SIZE;
						pages[page_idx / BUCKET_SIZE] &= ~(1ULL << (page_idx % BUCKET_SIZE));
						curr_addr += PAGE_SIZE;
					}
					return (void *)(bit_base_addr + hhdm_start);
				}
			}
		}

		bucket_base_addr += BUCKET_SIZE * PAGE_SIZE;
	}

	return NULL;
}

void free_pages(void *addr) {
	uintptr_t phys_addr = (uintptr_t)addr - hhdm_start;

	if (phys_addr % PAGE_SIZE != 0) {
		return;
	}

	if (phys_addr < phys_page_base) {
		return;
	}

	int page_off = (phys_addr - phys_page_base) / PAGE_SIZE;

	int bucket_idx = page_off / BUCKET_SIZE;
	int bit_idx = page_off % BUCKET_SIZE;

	if (bucket_idx >= TOTAL_BUCKET_COUNT) {
		return;
	}

	pages[bucket_idx] |= 1ULL << bit_idx;
}
