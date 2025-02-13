#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>

#include "../limine.h"

void init_screen(struct limine_framebuffer *framebuffer);

uint32_t get_rgb_color(uint8_t red, uint8_t green, uint8_t blue);
void draw_pixel(uint32_t color, uint64_t pos_x, uint64_t pos_y);
void clear_screen(uint32_t color);
#endif  // SCREEN_H
