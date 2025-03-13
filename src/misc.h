#ifndef MISC_H
#define MISC_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define RET_ERR 1
#define RET_OK 0

#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))
#define ALIGN_DOWN(x, align) ((x) & ~((align) - 1))

void stop(void);
void wait_for_interrupts(void);
size_t log2(size_t x);
size_t round_pow2(size_t x);
#endif  // MISC_H
