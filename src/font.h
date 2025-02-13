#ifndef FONT_H
#define FONT_H
#include <stdbool.h>

#include <stdint.h>

#define MAX_GLYPH_HEIGHT 10
#define MAX_GLYPH_WIDTH 10

struct Glyph {
	int width;
	int height;
	int x_off;
	int y_off;
	int advance;
	bool data[MAX_GLYPH_HEIGHT][MAX_GLYPH_WIDTH];
};

extern struct Glyph glyph_cap_a;
extern struct Glyph glyph_cap_b;
extern struct Glyph glyph_cap_c;
extern struct Glyph glyph_cap_d;
extern struct Glyph glyph_cap_e;
extern struct Glyph glyph_cap_f;
extern struct Glyph glyph_cap_g;
extern struct Glyph glyph_cap_h;
extern struct Glyph glyph_cap_i;
extern struct Glyph glyph_cap_j;
extern struct Glyph glyph_cap_k;
extern struct Glyph glyph_cap_l;
extern struct Glyph glyph_cap_m;
extern struct Glyph glyph_cap_n;
extern struct Glyph glyph_cap_o;
extern struct Glyph glyph_cap_p;
extern struct Glyph glyph_cap_q;
extern struct Glyph glyph_cap_r;
extern struct Glyph glyph_cap_s;
extern struct Glyph glyph_cap_t;
extern struct Glyph glyph_cap_u;
extern struct Glyph glyph_cap_v;
extern struct Glyph glyph_cap_w;
extern struct Glyph glyph_cap_x;
extern struct Glyph glyph_cap_y;
extern struct Glyph glyph_cap_z;

void draw_glyph(struct Glyph glyph, uint32_t color);

#endif  // FONT_H
