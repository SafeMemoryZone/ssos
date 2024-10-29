#include "vga_screen.h"

#include "ports.h"
#include "strlib.h"
#include "types.h"

#define VGA_BUFFER_ADDR 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define VGA_END_ADDR (VGA_BUFFER_ADDR + 2 * ((MAX_ROWS - 1) * MAX_COLS + MAX_COLS - 1))
#define IO_PORT_SCREEN_CTRL 0x3d4
#define IO_PORT_SCREEN_DATA 0x3d5

u16 get_cursor_screen_off() {
  port_byte_out(IO_PORT_SCREEN_CTRL, 14);
  u16 off = port_byte_in(IO_PORT_SCREEN_DATA) << 8;
  port_byte_out(IO_PORT_SCREEN_CTRL, 15);
  off += port_byte_in(IO_PORT_SCREEN_DATA);

  return off;
}

void set_cursor_screen_off(u16 off) {
  port_byte_out(IO_PORT_SCREEN_CTRL, 14);
  port_byte_out(IO_PORT_SCREEN_DATA, (u8)(off >> 8));
  port_byte_out(IO_PORT_SCREEN_CTRL, 15);
  port_byte_out(IO_PORT_SCREEN_DATA, (u8)(off & 0xff));
}

static inline char *get_vga_addr_from(u8 row, u8 col) {
  return (char *)VGA_BUFFER_ADDR + 2 * (row * MAX_COLS + col);
}

static inline u8 get_screen_off_row(u16 off) { return off / MAX_COLS; }

static inline u8 get_screen_off_col(u16 off) { return off % MAX_COLS; }

static inline u16 cell_add_to_screen_off(char *addr) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
  return (u16)(addr - VGA_BUFFER_ADDR) / 2;
#pragma GCC diagnostic pop
}

void clear_screen() {
  u16 screen_size = MAX_ROWS * MAX_COLS;

  for (int i = 0; i < screen_size; i++) {
    char *curr_cell_addr = (char *)VGA_BUFFER_ADDR + i * 2;
    *curr_cell_addr = ' ';
    curr_cell_addr[1] = VGA_DEFAULT_STYLE;
  }

  set_cursor_screen_off(0);
}

void scroll_screen() {
  for (int row = 0; row < MAX_ROWS - 1; row++) {
    for (int col = 0; col < MAX_COLS; col++) {
      char *curr_cell_addr = get_vga_addr_from(row, col);
      char *next_cell_addr = get_vga_addr_from(row + 1, col);

      *curr_cell_addr = *next_cell_addr;
      curr_cell_addr[1] = next_cell_addr[1];
    }
  }

  for (int col = 0; col < MAX_COLS; col++) {
    char *curr_cell_addr = get_vga_addr_from(MAX_ROWS - 1, col);
    *curr_cell_addr = ' ';
    curr_cell_addr[1] = VGA_DEFAULT_STYLE;
  }
}

void kprint_at(u8 start_row, u8 start_col, char *str, u8 style) {
  char *curr_cell_addr = get_vga_addr_from(start_row, start_col);

  while (*str) {
    if (*str == '\n') {
      u16 curr_off = cell_add_to_screen_off(curr_cell_addr);
      curr_cell_addr = get_vga_addr_from(get_screen_off_row(curr_off) + 1, 0);
    }
    else {
      *curr_cell_addr = *str;
      curr_cell_addr[1] = style;
      curr_cell_addr += 2;
    }

    if (curr_cell_addr > (char *)VGA_END_ADDR) {
      scroll_screen();
      curr_cell_addr = get_vga_addr_from(MAX_ROWS - 1, 0);
    }

    str++;
  }

  set_cursor_screen_off(cell_add_to_screen_off(curr_cell_addr));
}

void kprint(char *str, u8 style) {
  u16 curr_off = get_cursor_screen_off();
  kprint_at(get_screen_off_row(curr_off), get_screen_off_col(curr_off), str, style);
}

void kprint_dec(u32 num, u8 style) {
  char str[11];  // maximum 10 digits + null
  int i = 0;

  if (num == 0) {
    str[i++] = '0';
  }
  else {
    while (num) {
      str[i] = num % 10 + '0';
      i++;
      num /= 10;
    }
  }

  str[i] = 0;
  reverse_str(str);
  kprint(str, style);
}
