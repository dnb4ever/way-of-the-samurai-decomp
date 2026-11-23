/* upper.h -- game objects and globals of the code above 0x168000 whose module is not known yet */
#ifndef GAME_UPPER_H
#define GAME_UPPER_H

#include "common.h"
#include "game/unsorted.h"

/*
 * The types below are named after the first function, or the vtable, that
 * shows them. Only the members those functions use are known; the ranges
 * between them are kept as placeholder byte arrays named by offset.
 */

/* the object whose first word func_00168B60 returns: the string compared */
typedef struct Obj_00168B60 {
    char *unk_00;
} Obj_00168B60;

typedef struct Obj_00171FA0 {
    s32 unk_00;
} Obj_00171FA0;

/* a value at 0x24 that is copied, added and subtracted as a float */
typedef struct Obj_001756F0 {
    u8  unk_00[0x24];
    f32 unk_24;
} Obj_001756F0;

/* a value at 0x24 compared as a signed int */
typedef struct Obj_00175B60 {
    u8  unk_00[0x24];
    s32 unk_24;
} Obj_00175B60;

typedef struct Obj_00179620 {
    s32 unk_00;
} Obj_00179620;

/* read by one caller through func_00189140 and func_00189130 */
typedef struct Obj_00189130 {
    u8  unk_000[0x9];
    u8  unk_009;
    u8  unk_00A[0x15A];
    s32 unk_164;
} Obj_00189130;

/* func_0018A6A0 and func_0018A6B0 return the parts at 0x50 and 0x110 */
typedef struct Obj_0018A6A0 {
    u8 unk_000[0x50];
    u8 unk_050[0xC0];
    u8 unk_110[1];
} Obj_0018A6A0;

/* the class of vtable D_00223DA0 (slots func_00190970, func_00190980) */
typedef struct Obj_00223DA0 {
    u8  unk_00[0x20];
    s32 unk_20;
} Obj_00223DA0;

typedef struct Obj_00190A90 {
    u8  unk_00[0x10];
    s32 unk_10;
} Obj_00190A90;

typedef struct Obj_00190C10 {
    u32 unk_00;
} Obj_00190C10;

typedef struct Obj_00191340 {
    void *unk_00;               /* an object with a vtable at 0 */
} Obj_00191340;

/* the class of vtable D_00223DF0 (slots func_00199F20, func_00199F30) */
typedef struct Obj_00223DF0 {
    u8  unk_00[0x14];
    s32 unk_14;
} Obj_00223DF0;

typedef struct Obj_0019A210 {
    s16 unk_00;
} Obj_0019A210;

typedef struct Obj_001A6920 {
    s32 unk_00;
    u8  unk_04[0x8];
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    u8  unk_18[0x4];
    s16 unk_1C;
    u8  unk_1E[0x2];
    s16 unk_20;
    u8  unk_22[0x6];
    s32 unk_28;
} Obj_001A6920;

/* the part of Obj_001A7250 at its offset 0 */
typedef struct Obj_001A73D0 {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
} Obj_001A73D0;

typedef struct Obj_001A7250 {
    Obj_001A73D0 *unk_000;
    s32          unk_004;
    void        *unk_008;
    s32          unk_00C;
    s32          unk_010;
    s32          unk_014;
    s32          unk_018;
    s32          unk_01C;
    u8           unk_020[0x114];
    s32          unk_134;
    s32          unk_138;
    s32          unk_13C;
    s32          unk_140;
    s32          unk_144;
} Obj_001A7250;

/* 24 bytes, built by func_001B5300 */
typedef struct Obj_001B5300 {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} Obj_001B5300;

typedef struct Obj_001B5C80 {
    u8  unk_000[0x150];
    s32 unk_150;
} Obj_001B5C80;

/* the class of vtable D_00223E90 (slots func_001B5A50, func_001B5C80) */
typedef struct Obj_00223E90 {
    u8           unk_00[0x4];
    Obj_001B5C80 *unk_04;
} Obj_00223E90;

typedef struct Obj_001B6990 {
    u8  unk_000[0xCE0];
    u8  unk_CE0[0x10];
    s32 unk_CF0;
    u8  unk_CF4[0x1C];
    f32 unk_D10;
    f32 unk_D14;
    f32 unk_D18;
    f32 unk_D1C;
} Obj_001B6990;


typedef struct Obj_001BB400 {
    u8  unk_000[0x924];
    s32 unk_924;
    s32 unk_928;
    s32 unk_92C;
    u8  unk_930[0x110];
    u8  unk_A40[1];
} Obj_001BB400;

/* 8 bytes, built by func_001EB3C0 */
typedef struct Obj_001EB3C0 {
    s32 unk_00;
    s32 unk_04;
} Obj_001EB3C0;

/* the class of vtable D_002240A0 (slot func_001F7130) */
typedef struct Obj_002240A0 Obj_002240A0;

/* small data */
extern u8  D_00224760;
extern s32 D_002247A0;
extern s32 D_002247B8;
extern s32 D_002247BC;
extern s32 D_002247C0;
extern s32 D_002249F8;
extern u8  D_00224A00;
extern s32 D_00224A08;
extern s32 D_00224A10;
extern s32 D_00224A14;
extern s32 D_00224A18;
extern s32 D_00224A1C;
extern s32 D_00224A20;
extern s32 D_00224A24;
extern s32 D_00224A2C;
extern s32 D_00224A30;

/* functions, by address */
char *func_00168B60(Obj_00168B60 *obj);
s32 func_00171FA0(Obj_00171FA0 *obj);
void func_001756F0(Obj_001756F0 *dst, Obj_001756F0 *src);
void func_00175740(Obj_001756F0 *dst, Obj_001756F0 *src);
void func_00175760(Obj_001756F0 *dst, Obj_001756F0 *src);
s32 func_00175B60(Obj_00175B60 *a, Obj_00175B60 *b);
s32 func_00175B70(Obj_00175B60 *a, Obj_00175B60 *b);
s32 func_00175B90(Obj_00175B60 *a, Obj_00175B60 *b);
s32 func_00179620(Obj_00179620 *obj, s32 n);
void func_001886A0(s32 value);
s32 func_00189130(Obj_00189130 *obj);
u8 func_00189140(Obj_00189130 *obj);
void *func_0018A6A0(Obj_0018A6A0 *obj);
void *func_0018A6B0(Obj_0018A6A0 *obj);
void func_0018E3E0(s32 value);
void func_0018E3F0(s32 value);
void func_0018E400(s32 value);
void func_0018E410(s32 value);
void func_0018E420(s32 value);
void func_0018E430(s32 value);
void func_0018E440(s32 value);
void func_0018F0B0(u8 value);
s32 func_00190970(Obj_00223DA0 *obj);
s32 func_00190980(Obj_00223DA0 *obj);
void func_00190A90(Obj_00190A90 *obj);
void *func_00190AA0(Obj_001485B0 *obj, char *name);
s32 func_00190C10(Obj_00190C10 *obj, u32 n);
void func_00190CB0(Obj_00191340 *obj, void *other);
void func_00191340(Obj_00191340 *obj);
s32 func_00199F20(Obj_00223DF0 *obj);
s32 func_00199F30(Obj_00223DF0 *obj);
s16 func_0019A210(Obj_0019A210 *obj);
void func_0019CFC0(u8 value);
void func_0019CFD0(void);
void func_0019CFE0(void);
void func_001A6920(Obj_001A6920 *obj);
void func_001A7250(Obj_001A7250 *obj);
void func_001A73D0(Obj_001A73D0 *obj, s32 value);
void func_001A73E0(Obj_001A73D0 *obj, s32 value);
s32 func_001A7430(Obj_001A7250 *obj);
s32 func_001A7440(Obj_001A7250 *obj);
void func_001A7450(Obj_001A73D0 *obj, s32 value);
void func_001A74E0(Obj_001A7250 *obj, void *ptr);
void func_001B43C0(void);
void func_001B5240(void);
Obj_001B5300 *func_001B5300(Obj_001B5300 *obj);
s32 func_001B5A50(Obj_00223E90 *obj);
s32 func_001B5C80(Obj_00223E90 *obj, s32 value);
void func_001B5ED0(void);
void func_001B6140(s32 value);
s32 func_001B6990(Obj_001B6990 *obj, f32 value);
s32 func_001B69A0(Obj_001B6990 *obj, f32 value);
s32 func_001B69B0(Obj_001B6990 *obj, f32 value);
s32 func_001B69C0(Obj_001B6990 *obj, f32 value);
void *func_001B69D0(Obj_001B6990 *obj);
s32 func_001B69E0(Obj_001B6990 *obj);
void func_001B9D70(void *part, s32 arg1, s32 arg2, s32 arg3);
s32 func_001BB400(Obj_001BB400 *obj, s32 arg1, s32 arg2, s32 arg3);
void func_001BB4B0(Obj_001BB400 *obj, s32 a, s32 b, s32 c);
s32 func_001BBAA0(Obj_001BB400 *obj);
void func_001E4300(s32 a, s32 b, s32 c);
Obj_001EB3C0 *func_001EB3C0(Obj_001EB3C0 *obj);
void func_001F7130(Obj_002240A0 *obj);
void *func_001F8E70(void *ptr);

#endif /* GAME_UPPER_H */
