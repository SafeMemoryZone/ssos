#ifndef STDINT_H
#define STDINT_H

typedef signed char        int8_t;
typedef unsigned char      uint8_t;

typedef signed short       int16_t;
typedef unsigned short     uint16_t;

typedef signed int         int32_t;
typedef unsigned int       uint32_t;

typedef signed long        int64_t;
typedef unsigned long      uint64_t;

typedef int8_t             int_least8_t;
typedef uint8_t            uint_least8_t;

typedef int16_t            int_least16_t;
typedef uint16_t           uint_least16_t;

typedef int32_t            int_least32_t;
typedef uint32_t           uint_least32_t;

typedef int64_t            int_least64_t;
typedef uint64_t           uint_least64_t;

typedef int64_t            int_fast8_t;
typedef uint64_t           uint_fast8_t;

typedef int64_t            int_fast16_t;
typedef uint64_t           uint_fast16_t;

typedef int64_t            int_fast32_t;
typedef uint64_t           uint_fast32_t;

typedef int64_t            int_fast64_t;
typedef uint64_t           uint_fast64_t;

typedef signed long        intptr_t;
typedef unsigned long      uintptr_t;

typedef int64_t            intmax_t;
typedef uint64_t           uintmax_t;

#define INT8_MIN           (-128)
#define INT8_MAX           127
#define UINT8_MAX          255

#define INT16_MIN          (-32768)
#define INT16_MAX          32767
#define UINT16_MAX         65535

#define INT32_MIN          (-2147483648)
#define INT32_MAX          2147483647
#define UINT32_MAX         4294967295U

#define INT64_MIN          (-9223372036854775807L - 1)
#define INT64_MAX          9223372036854775807L
#define UINT64_MAX         18446744073709551615UL

#define INT_LEAST8_MIN     INT8_MIN
#define INT_LEAST8_MAX     INT8_MAX
#define UINT_LEAST8_MAX    UINT8_MAX

#define INT_LEAST16_MIN    INT16_MIN
#define INT_LEAST16_MAX    INT16_MAX
#define UINT_LEAST16_MAX   UINT16_MAX

#define INT_LEAST32_MIN    INT32_MIN
#define INT_LEAST32_MAX    INT32_MAX
#define UINT_LEAST32_MAX   UINT32_MAX

#define INT_LEAST64_MIN    INT64_MIN
#define INT_LEAST64_MAX    INT64_MAX
#define UINT_LEAST64_MAX   UINT64_MAX

#define INT_FAST8_MIN      INT64_MIN
#define INT_FAST8_MAX      INT64_MAX
#define UINT_FAST8_MAX     UINT64_MAX

#define INT_FAST16_MIN     INT64_MIN
#define INT_FAST16_MAX     INT64_MAX
#define UINT_FAST16_MAX    UINT64_MAX

#define INT_FAST32_MIN     INT64_MIN
#define INT_FAST32_MAX     INT64_MAX
#define UINT_FAST32_MAX    UINT64_MAX

#define INT_FAST64_MIN     INT64_MIN
#define INT_FAST64_MAX     INT64_MAX
#define UINT_FAST64_MAX    UINT64_MAX

#define INTPTR_MIN         INT64_MIN
#define INTPTR_MAX         INT64_MAX
#define UINTPTR_MAX        UINT64_MAX

#define INTMAX_MIN         INT64_MIN
#define INTMAX_MAX         INT64_MAX
#define UINTMAX_MAX        UINT64_MAX

#define PTRDIFF_MIN        INT64_MIN
#define PTRDIFF_MAX        INT64_MAX

#define SIZE_MAX           UINT64_MAX

#define WCHAR_MIN          0
#define WCHAR_MAX          0x7FFFFFFF

#define WINT_MIN           0
#define WINT_MAX           0x7FFFFFFF

#define INT8_C(value)      value
#define UINT8_C(value)     value##U

#define INT16_C(value)     value
#define UINT16_C(value)    value##U

#define INT32_C(value)     value
#define UINT32_C(value)    value##U

#define INT64_C(value)     value##L
#define UINT64_C(value)    value##UL

#define INTMAX_C(value)    INT64_C(value)
#define UINTMAX_C(value)   UINT64_C(value)

#endif // STDINT_H
