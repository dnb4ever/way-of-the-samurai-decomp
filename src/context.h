/* context.h for SLUS-20407: shared by every function. Agents grow it with edit_context:
 * add a prototype or struct the moment you work one out, so the next function benefits. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile s32 vs32;
typedef int s32_bool;

// Function prototypes for small functions
void func_00147940(u8 *dst, const u8 *src, u32 len);
u16 func_00147950(u16 val);
s32 func_001478F0(u32 flags, u32 mask);
s32 func_00147A20(const char *a, const char *b);

#define NULL ((void *)0)
#define TRUE 1
#define FALSE 0
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

/* This game keeps small globals (8 bytes or less) in $gp-relative small data. Declare those here WITHOUT
 * `extern` (for example `s32 D_8001B278;`): then the compiler reads them through $gp, like the game does.
 * With `extern`, it builds the full address instead and the function can't match. */

/* Small data declarations (placed in $gp-relative area) */
extern u8 D_00223650[];

/* Function prototypes */
void func_00100480(void *arg0);
void func_001033F8(u32, u32);
void func_0010EEE0(u32);
void func_00105C50(u32, u32, u32);
void func_00105C98(u32, u32, f32);
void func_0015FAC0(u32);
void func_0013A8B0(void);

/* Data symbols - declare as u8 arrays to force absolute addressing (%hi/%lo) */
extern u8 D_00223750[];

s32 func_001DF320(f32);
void func_0011DC50(f32);

/* func_0011DC90: prototype for called function */
void func_0011DC90(u32);

s32 func_001555F0(u32 arg);
void func_001555B0(u32 arg);
u32 func_001555A0(u32 arg);
void func_00155590(void);

void syscall_0x5b(void);
void syscall_0x5c(void);
void syscall_0x59(void);

extern u8 D_00204F88[];
extern u8 D_00204C00[];
