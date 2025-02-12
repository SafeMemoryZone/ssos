#ifndef MISC_H
#define MISC_H
#include <stddef.h>
#include <stdint.h>

void reverse_str(char *str);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
size_t strlen(const char *str);

void stop(void);
#endif  // MISC_H
