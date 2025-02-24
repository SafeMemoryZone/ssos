#ifndef PAGING_H
#define PAGING_H
#include <stddef.h>

#include "../limine.h"

void init_page_frame_allocator(struct limine_memmap_entry *mmap_entries, size_t entry_count);
void *alloc_pages(size_t count);
void free_pages(void *addr);
#endif  // PAGING_H
