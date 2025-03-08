#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>

#include "../limine.h"

// Misc
void init_screen(struct limine_framebuffer *framebuffer);
uint32_t get_rgb_color(uint32_t red, uint32_t green, uint32_t blue);
void set_color(uint32_t color);
void set_font_scale(int scale);

// Pixel drawing functions
void draw_pixel(uint64_t pos_x, uint64_t pos_y);
void clear_screen(void);

// Higher level printing functions
void kputch(char ch);
void kprint(char *str);
void kprint_addr(uintptr_t addr);

// View manipulation
void scroll_screen(unsigned int pixel_count);
#endif  // SCREEN_H
