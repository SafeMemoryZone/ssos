#ifndef MISC_H
#define MISC_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))
#define ALIGN_DOWN(x, align) ((x) & ~((align) - 1))

void stop(void);
void assert(bool cond, char *msg);
size_t log2(size_t x);
size_t round_pow2(size_t x);
#endif  // MISC_H
