#include "misc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

__attribute__((noreturn)) void stop(void) {
	asm volatile("cli");
	while (1) {
		asm volatile("hlt");
	}
}

void wait_for_interrupts(void) { __asm__ __volatile__("hlt"); }

size_t log2(size_t x) { return 31U - __builtin_clz(x); }

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
