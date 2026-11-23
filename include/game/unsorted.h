/* unsorted.h -- game code at 0x11C000-0x167FFF whose module is not known */
#ifndef GAME_UNSORTED_H
#define GAME_UNSORTED_H

#include "common.h"

/*
 * Types here are named after the vtable (Class_XXXXXXXX, vtable
 * D_XXXXXXXX) or after a function that uses them (Obj_XXXXXXXX). Members
 * are named by offset; only the members recovered code touches are
 * declared, the ranges between them are byte arrays.
 */

/* ---- small globals ($gp-relative) ---- */

extern s32 D_002246E0;
extern s32 D_00224718;
extern s32 D_00224898;
extern s32 D_002248E0;

s32  func_00133D00(void);
void func_00147430(s32 value);
void func_0014A9F0(s32 value);
s32  func_00159FC0(s32 value);

/* ---- objects with a vtable ---- */

typedef struct Class_00223740 Class_00223740;
typedef struct Class_00223A70 Class_00223A70;

/* derives from the class of vtable D_002236A0 */
typedef struct Class_00223DA0 {
    const struct Class_00223DA0Vtbl *vtbl;  /* 0x00 */
    u8                               unk_04[0x14];
    s32                              unk_18;  /* 0x18 */
} Class_00223DA0;

/* derives from the class of vtable D_00223750 */
typedef struct Class_00223970 {
    u8                               unk_000[0xC];
    const struct Class_00223970Vtbl *vtbl;     /* 0x00C */
    u8                               unk_010[0x400];
    s32                              unk_410;  /* 0x410 */
    u8                               unk_414[0x21];
    u8                               unk_435;  /* 0x435 */
    u8                               unk_436[2];
    u8                               unk_438;  /* 0x438 */
} Class_00223970;

void func_00147A10(Class_00223740 *obj);  /* slot 1 */
void func_00163CC0(Class_00223A70 *obj);  /* slot 1 */
s32  func_001338C0(Class_00223DA0 *obj);  /* slot 5 */
void func_00144090(Class_00223970 *obj, s32 value);
void func_001440A0(Class_00223970 *obj, s32 value435, s32 value410);

/* ---- other objects ---- */

typedef struct Obj_0011C7B0 {
    u8  unk_00[0x84];
    s32 unk_84;  /* 0x84 */
} Obj_0011C7B0;

typedef struct Obj_001201D0 {
    u8 unk_000[0xDC0];
    u8 unk_DC0;  /* 0xDC0 */
} Obj_001201D0;

typedef struct Obj_0013A2F0 {
    u8  unk_000[0x100];
    u64 unk_100;  /* 0x100 */
    u8  unk_108[0xE8];
    u64 unk_1F0;  /* 0x1F0 */
} Obj_0013A2F0;

typedef struct Obj_0013E1C0 {
    s32 unk_00;  /* 0x00 */
} Obj_0013E1C0;

typedef struct Obj_0013FC50 {
    u8 unk_00[0x28];
    u8 unk_28;  /* 0x28 */
} Obj_0013FC50;

typedef struct Obj_001487E0Sub {
    u8  unk_00[4];
    u16 unk_04;  /* 0x04 */
} Obj_001487E0Sub;

typedef struct Obj_001487E0 {
    u8              unk_00[8];
    Obj_001487E0Sub unk_08;  /* 0x08 */
} Obj_001487E0;

typedef struct Obj_00149D30 {
    s32 unk_00;  /* 0x00 */
} Obj_00149D30;

typedef struct Obj_00154DE0 {
    u8  unk_000[0x274];
    s32 unk_274;  /* 0x274 */
    u8  unk_278[4];
    s32 unk_27C;  /* 0x27C */
} Obj_00154DE0;

typedef struct Obj_00155640 {
    s32 unk_00;  /* 0x00 */
    u8  unk_04;  /* 0x04 */
} Obj_00155640;

typedef struct Obj_00155730 {
    u8  unk_00[0x14];
    s32 unk_14;  /* 0x14 */
    u8  unk_18[0x28];
    s16 unk_40;  /* 0x40 */
    s16 unk_42;  /* 0x42 */
} Obj_00155730;

typedef struct Obj_0015E6C0 {
    u8  unk_00[0x1C];
    s32 unk_1C;  /* 0x1C */
} Obj_0015E6C0;

typedef struct Obj_00161BE0 Obj_00161BE0;
typedef struct Obj_00161BE0Arg {
    u8  unk_00[0x4C];
    s32 unk_4C;  /* 0x4C */
} Obj_00161BE0Arg;

typedef struct Obj_001485B0 Obj_001485B0;
typedef struct Obj_0011DEB0 Obj_0011DEB0;
typedef struct Obj_001335B0 Obj_001335B0;
typedef struct Obj_001350A0 Obj_001350A0;

s32  func_001F71D0(s32 arg);
void func_0011C7B0(Obj_0011C7B0 *obj, s32 arg);
void func_0011DC90(Obj_0011DEB0 *obj);
Obj_0011DEB0 *func_0011DEB0(Obj_0011DEB0 *obj);
u8   func_001201D0(Obj_001201D0 *obj);
void func_00133420(Obj_001335B0 *obj);
Obj_001335B0 *func_001335B0(Obj_001335B0 *obj);
void func_001350D0(Obj_001350A0 *obj);
Obj_001350A0 *func_001350A0(Obj_001350A0 *obj);
void func_0013A2F0(Obj_0013A2F0 *obj, u32 value);
s32  func_0013E1C0(Obj_0013E1C0 *obj);
void func_0013FC50(Obj_0013FC50 *obj, s32 value);
s32  func_00199C50(Obj_001485B0 *obj, char *name);
void func_001485B0(Obj_001485B0 *obj, char *name);
Obj_001487E0Sub *func_001487E0(Obj_001487E0 *obj);
s32  func_00149D30(Obj_00149D30 *obj, s32 value);
void func_00149E60(Obj_001487E0 *obj);
s32  func_00149E00(Obj_001487E0 *obj);
s32  func_00154DE0(Obj_00154DE0 *obj);
s32  func_00154E00(Obj_00154DE0 *obj);
Obj_00155640 *func_00155640(Obj_00155640 *obj, s32 value);
s16  func_00155730(Obj_00155730 *obj);
s32  func_00156690(Obj_00155730 *obj);
s16  func_001566A0(Obj_00155730 *obj, s32 value);
s32  func_0015E6C0(Obj_0015E6C0 *obj, s32 value);
void func_0015FE30(void);
void func_0015FE40(void);
s32  func_00161BE0(Obj_00161BE0 *obj, Obj_00161BE0Arg *arg);

#endif /* GAME_UNSORTED_H */
