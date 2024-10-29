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
#endif  // TYPES_H
