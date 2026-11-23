/* common.h -- base types shared by every unit of SLUS_204.07 */
#ifndef COMMON_H
#define COMMON_H

typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;

/* the R5900's 128-bit general registers (GCC's TImode) */
typedef unsigned int u128 __attribute__((mode(TI)));

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif /* COMMON_H */
