#ifndef MEM_H
#define MEM_H
#include <stddef.h>

int kalloc_init(void);

void *kmalloc(size_t bytes);
void kfree(void *mem);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
size_t strlen(const char *str);
#endif  // MEM_H
