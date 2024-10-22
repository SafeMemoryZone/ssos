#include "ports.h"

uint8_t port_byte_in(uint16_t port) {
  uint8_t result;
  __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
  return result;
}

void port_byte_out(uint16_t port, uint8_t byte) {
  __asm__("out %%al, %%dx" : : "a"(byte), "d"(port));
}

uint16_t port_word_in(uint16_t port) {
  uint16_t result;
  __asm__("in %%dx, %%ax" : "=a"(result) : "d"(port));
  return result;
}

void port_word_out(uint16_t port, uint16_t word) {
  __asm__("out %%ax, %%dx" : : "a"(word), "d"(port));
}
