#include "ports.h"

#include <stdint.h>

inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ __volatile__("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}

inline void outb(uint16_t port, uint8_t val) {
	__asm__ __volatile__("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline void io_wait(void) { outb(0x80, 0); }
