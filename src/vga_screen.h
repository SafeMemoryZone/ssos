#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>

#define VGA_GET_STYLE(fg, bg) (fg | bg << 4)
#define VGA_DEFAULT_STYLE VGA_GET_STYLE(VGA_WHITE, VGA_BLACK)

enum VGAColor {
  VGA_BLACK = 0,
  VGA_BLUE = 1,
  VGA_GREEN = 2,
  VGA_CYAN = 3,
  VGA_RED = 4,
  VGA_MAGENTA = 5,
  VGA_BROWN = 6,
  VGA_LIGHT_GREY = 7,
  VGA_DARK_GREY = 8,
  VGA_LIGHT_BLUE = 9,
  VGA_LIGHT_GREEN = 10,
  VGA_LIGHT_CYAN = 11,
  VGA_LIGHT_RED = 12,
  VGA_LIGHT_MAGENTA = 13,
  VGA_LIGHT_BROWN = 14,
  VGA_WHITE = 15,
};

void clear_screen();
void kprint_at(uint8_t start_row, uint8_t start_col, char *str, uint8_t style);
void kprint(char *str, uint8_t style);
void kprint_default(char *str);
void kprint_dec(uint32_t num, uint8_t style);
void kprint_dec_default(uint32_t num);
#endif // SCREEN_H
