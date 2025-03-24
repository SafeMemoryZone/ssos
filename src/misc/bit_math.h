#ifndef BIT_MATH_H
#define BIT_MATH_H
#include <stdint.h>

#include "mem/paging.h"

#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))
#define ALIGN_DOWN(x, align) ((x) & ~((align) - 1))

uint64_t logb(uint64_t x);
uint64_t round_pow2_boundary(uint64_t x);
uint32_t calculate_ccitt32_ansi_crc32(const void *data, size_t length);
#endif  // BIT_MATH_H
