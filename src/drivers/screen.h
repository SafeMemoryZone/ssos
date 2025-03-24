#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>

#include "limine.h"

void screen_init(struct limine_framebuffer *framebuffer);
uint32_t screen_get_rgb_color(uint32_t red, uint32_t green, uint32_t blue);
void screen_set_color(uint32_t color);
void screen_set_font_scale(int scale);
void screen_putch(char ch);
void screen_print(char *str);
void screen_print_addr(uintptr_t addr);
void screen_scroll(unsigned int pixel_count);
void screen_clear(void);
#endif  // SCREEN_H
