#include "bit_math.h"

#include <stdint.h>

#include "mem/paging.h"

uint64_t logb(uint64_t x) { return 31U - __builtin_clz(x); }

uint64_t round_pow2_boundary(uint64_t x) {
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;

	return x;
}

uint32_t calculate_ccitt32_ansi_crc32(const void *data, size_t length) {
	uint32_t crc = 0xFFFFFFFF;
	const uint8_t *data_cast = (uint8_t *)data;

	while (length--) {
		crc ^= *data_cast++;
		for (unsigned int i = 0; i < 8; i++) {
			if (crc & 1)
				crc = (crc >> 1) ^ 0xEDB88320;
			else
				crc >>= 1;
		}
	}
	return crc ^ 0xFFFFFFFF;
}
