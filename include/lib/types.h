#ifndef TYPES_H
#define TYPES_H

typedef signed char i8;
typedef unsigned char u8;
typedef signed short i16;
typedef unsigned short u16;
typedef signed int i32;
typedef unsigned int u32;
typedef int bool;
typedef u32 size_t;

#define I8_MIN (-128)
#define I8_MAX 127
#define U8_MAX 255

#define I16_MIN (-32768)
#define I16_MAX 32767
#define U16_MAX 65535

#define I32_MIN (-2147483648)
#define I32_MAX 2147483647
#define U32_MAX 4294967295U

_Static_assert(sizeof(i8) == 1, "i8 size is not 1 byte");
_Static_assert(sizeof(u8) == 1, "u8 size is not 1 byte");
_Static_assert(sizeof(i16) == 2, "i16 size is not 2 bytes");
_Static_assert(sizeof(u16) == 2, "u16 size is not 2 bytes");
_Static_assert(sizeof(i32) == 4, "i32 size is not 4 bytes");
_Static_assert(sizeof(u32) == 4, "u32 size is not 4 bytes");
#endif  // TYPES_H
