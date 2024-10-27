#include "ports.h"

u8 port_byte_in(u16 port) {
  u8 result;
  __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
  return result;
}

void port_byte_out(u16 port, u8 byte) {
  __asm__("out %%al, %%dx" : : "a"(byte), "d"(port));
}

u16 port_word_in(u16 port) {
  u16 result;
  __asm__("in %%dx, %%ax" : "=a"(result) : "d"(port));
  return result;
}

void port_word_out(u16 port, u16 word) {
  __asm__("out %%ax, %%dx" : : "a"(word), "d"(port));
}
