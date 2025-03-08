#ifndef PAGING_H
#define PAGING_H
#include <stddef.h>

#include "../limine.h"

#define PAGE_SIZE 0x200000
#define LOG2_PAGE_SIZE 21

int init_page_frame_allocator(struct limine_memmap_entry *mmap_entries, size_t entry_count, uint64_t hh_off);
void *alloc_pages(size_t count);
void free_pages(void *addr, size_t count);
#endif  // PAGING_H
