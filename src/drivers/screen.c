#include "screen.h"
#include "ports.h"
#include "../lib/stdint.h"

#define VGA_BUFFER_ADDR 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define IO_PORT_SCREEN_CTRL 0x3d4
#define IO_PORT_SCREEN_DATA 0x3d5

inline uint16_t get_screen_offset(uint8_t row, uint8_t col) {
  return (row * MAX_ROWS + col) * 2;
}

uint16_t get_cursor_offset() {
  port_byte_out(IO_PORT_SCREEN_CTRL, 14);
  uint16_t off = port_byte_in(IO_PORT_SCREEN_DATA) << 8;
  port_byte_out(IO_PORT_SCREEN_CTRL, 15);
  off += port_byte_in(IO_PORT_SCREEN_DATA);

  return off * 2;
}

void set_cursor_offset(uint16_t offset) {
  offset /= 2;
  port_byte_out(IO_PORT_SCREEN_CTRL, 14);
  port_byte_out(IO_PORT_SCREEN_DATA, (uint8_t)(offset >> 8));
  port_byte_out(IO_PORT_SCREEN_CTRL, 15);
  port_byte_out(IO_PORT_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

void clear_screen() {
  uint16_t screen_size = MAX_ROWS * MAX_COLS;

  for(int i = 0; i < screen_size; i++) {
    char *curr_char = (char *) VGA_BUFFER_ADDR + i * 2;
    *curr_char = ' ';
  }
}
