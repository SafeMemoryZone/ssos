#include "screen.h"

#include <stdint.h>

#include "../limine.h"
#include "../misc.h"

static struct limine_framebuffer *FRAMEBUFFER;

void init_screen(struct limine_framebuffer *framebuffer) { FRAMEBUFFER = framebuffer; }

uint32_t get_rgb_color(uint8_t red, uint8_t green, uint8_t blue) {
	return (red << FRAMEBUFFER->red_mask_shift) | (green << FRAMEBUFFER->green_mask_shift) |
	       (blue << FRAMEBUFFER->blue_mask_shift);
}

void draw_pixel(uint32_t color, uint64_t pos_x, uint64_t pos_y) {
	uint32_t *address =
	    FRAMEBUFFER->address + FRAMEBUFFER->pitch * pos_x + (FRAMEBUFFER->bpp / 8) * pos_y;
	*address = color;
}

void clear_screen(uint32_t color) {
	uint32_t *address = (uint32_t *)FRAMEBUFFER->address;
	uint64_t pixels_per_row = FRAMEBUFFER->pitch / 4;
	uint64_t total_pixels = pixels_per_row * FRAMEBUFFER->height;

	for (uint64_t i = 0; i < total_pixels; i++) {
		address[i] = color;
	}
}
