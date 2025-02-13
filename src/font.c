#include "font.h"

#include <stdbool.h>

static uint64_t curr_col_pixel;
static uint64_t curr_row_pixel;

// Glyph for A
struct Glyph glyph_cap_a = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {// Rows 0-6: the 5x7 design; rows 7-9 are all false.
             {false, false, true, false, false, false, false, false, false, false},  //  "  X  "
             {false, true, false, true, false, false, false, false, false, false},   //  " X X "
             {true, false, false, false, true, false, false, false, false, false},   //  "X   X"
             {true, true, true, true, true, false, false, false, false, false},      //  "XXXXX"
             {true, false, false, false, true, false, false, false, false, false},   //  "X   X"
             {true, false, false, false, true, false, false, false, false, false},   //  "X   X"
             {true, false, false, false, true, false, false, false, false, false},   //  "X   X"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for B
struct Glyph glyph_cap_b = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, true, true, true, false, false, false, false, false, false},    // "XXXX "
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, true, true, true, false, false, false, false, false, false},    // "XXXX "
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, true, true, true, false, false, false, false, false, false},    // "XXXX "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for C
struct Glyph glyph_cap_c = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{false, true, true, true, false, false, false, false, false, false},    // " XXX "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {false, true, true, true, false, false, false, false, false, false},    // " XXX "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for D
struct Glyph glyph_cap_d = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, true, true, true, false, false, false, false, false, false},    // "XXXX "
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, true, true, true, false, false, false, false, false, false},    // "XXXX "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for E
struct Glyph glyph_cap_e = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, true, true, true, true, false, false, false, false, false},      // "XXXXX"
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, true, true, true, false, false, false, false, false, false},     // "XXXX "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, true, true, true, true, false, false, false, false, false},      // "XXXXX"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for F
struct Glyph glyph_cap_f = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, true, true, true, true, false, false, false, false, false},      // "XXXXX"
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, true, true, true, false, false, false, false, false, false},     // "XXXX "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for G
struct Glyph glyph_cap_g = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{false, true, true, true, false, false, false, false, false, false},    // " XXX "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, true, true, false, false, false, false, false},    // "X  XX"
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {false, true, true, true, false, false, false, false, false, false},    // " XXX "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for H
struct Glyph glyph_cap_h = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, true, true, true, true, false, false, false, false, false},     // "XXXXX"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for I
struct Glyph glyph_cap_i = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{false, true, true, true, false, false, false, false, false, false},    // " XXX "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, true, true, true, false, false, false, false, false, false},    // " XXX "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for J
struct Glyph glyph_cap_j = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{false, false, true, true, true, false, false, false, false, false},    // "  XXX"
             {false, false, false, true, false, false, false, false, false, false},  // "   X "
             {false, false, false, true, false, false, false, false, false, false},  // "   X "
             {false, false, false, true, false, false, false, false, false, false},  // "   X "
             {false, false, false, true, false, false, false, false, false, false},  // "   X "
             {true, false, false, true, false, false, false, false, false, false},   // "X  X "
             {false, true, true, false, false, false, false, false, false, false},   // " XX  "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for K
struct Glyph glyph_cap_k = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, true, false, false, false, false, false, false},  // "X  X "
             {true, false, true, false, false, false, false, false, false, false},  // "X X  "
             {true, true, false, false, false, false, false, false, false, false},  // "XX   "
             {true, false, true, false, false, false, false, false, false, false},  // "X X  "
             {true, false, false, true, false, false, false, false, false, false},  // "X  X "
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for L
struct Glyph glyph_cap_l = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, true, true, true, true, false, false, false, false, false},      // "XXXXX"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for M
struct Glyph glyph_cap_m = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, true, false, true, true, false, false, false, false, false},    // "XX XX"
             {true, false, true, false, true, false, false, false, false, false},   // "X X X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for N
struct Glyph glyph_cap_n = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, true, false, false, true, false, false, false, false, false},   // "XX  X"
             {true, false, true, false, true, false, false, false, false, false},   // "X X X"
             {true, false, false, true, true, false, false, false, false, false},   // "X  XX"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for O
struct Glyph glyph_cap_o = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{false, true, true, true, false, false, false, false, false, false},   // " XXX "
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {false, true, true, true, false, false, false, false, false, false},   // " XXX "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for P
struct Glyph glyph_cap_p = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, true, true, true, false, false, false, false, false, false},     // "XXXX "
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {true, true, true, true, false, false, false, false, false, false},     // "XXXX "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for Q
struct Glyph glyph_cap_q = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{false, true, true, true, false, false, false, false, false, false},   // " XXX "
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, true, false, true, false, false, false, false, false},   // "X X X"
             {true, false, false, true, false, false, false, false, false, false},  // "X  X "
             {false, true, true, false, true, false, false, false, false, false},   // " XX X"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for R
struct Glyph glyph_cap_r = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, true, true, true, false, false, false, false, false, false},    // "XXXX "
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, true, true, true, false, false, false, false, false, false},    // "XXXX "
             {true, false, true, false, false, false, false, false, false, false},  // "X X  "
             {true, false, false, true, false, false, false, false, false, false},  // "X  X "
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for S
struct Glyph glyph_cap_s = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{false, true, true, true, true, false, false, false, false, false},     // " XXXX"
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {false, true, true, true, false, false, false, false, false, false},    // " XXX "
             {false, false, false, false, true, false, false, false, false, false},  // "    X"
             {false, false, false, false, true, false, false, false, false, false},  // "    X"
             {true, true, true, true, false, false, false, false, false, false},     // "XXXX "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for T
struct Glyph glyph_cap_t = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, true, true, true, true, false, false, false, false, false},      // "XXXXX"
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for U
struct Glyph glyph_cap_u = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {false, true, true, true, false, false, false, false, false, false},   // " XXX "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for V
struct Glyph glyph_cap_v = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {false, true, false, true, false, false, false, false, false, false},   // " X X "
             {false, true, false, true, false, false, false, false, false, false},   // " X X "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for W
struct Glyph glyph_cap_w = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {true, false, true, false, true, false, false, false, false, false},   // "X X X"
             {true, false, true, false, true, false, false, false, false, false},   // "X X X"
             {true, true, false, true, true, false, false, false, false, false},    // "XX XX"
             {true, false, false, false, true, false, false, false, false, false},  // "X   X"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for X
struct Glyph glyph_cap_x = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {false, true, false, true, false, false, false, false, false, false},   // " X X "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, true, false, true, false, false, false, false, false, false},   // " X X "
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for Y
struct Glyph glyph_cap_y = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {true, false, false, false, true, false, false, false, false, false},   // "X   X"
             {false, true, false, true, false, false, false, false, false, false},   // " X X "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

// Glyph for Z
struct Glyph glyph_cap_z = {
    .width = 5,
    .height = 7,
    .x_off = 0,
    .y_off = 0,
    .advance = 6,
    .data = {{true, true, true, true, true, false, false, false, false, false},      // "XXXXX"
             {false, false, false, false, true, false, false, false, false, false},  // "    X"
             {false, false, false, true, false, false, false, false, false, false},  // "   X "
             {false, false, true, false, false, false, false, false, false, false},  // "  X  "
             {false, true, false, false, false, false, false, false, false, false},  // " X   "
             {true, false, false, false, false, false, false, false, false, false},  // "X    "
             {true, true, true, true, true, false, false, false, false, false},      // "XXXXX"
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false},
             {false, false, false, false, false, false, false, false, false, false}}};

void draw_glyph(struct Glyph glyph, uint32_t color) {
    
}
