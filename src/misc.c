#include "misc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "drivers/screen.h"

void stop(void) {
	for (;;) {
		__asm__ __volatile__("hlt");
	}
}

void assert(bool cond, char *msg) {
	if (!cond) {
		kprint("[ASSERTION FAILURE] ");
		kprint(msg);
		stop();
	}
}

size_t log2(size_t x) {
	size_t result = 0;
	while (x >>= 1) {
		result++;
	}
	return result;
}

size_t pow(size_t base, int exp) {
	size_t result = 1;
	for (;;) {
		if (exp & 1) {
			result *= base;
		}
		exp >>= 1;

		if (!exp) {
			break;
		}

		base *= base;
	}

	return result;
}

size_t round_pow2(size_t x) {
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;

	return x;
}
