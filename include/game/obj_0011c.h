/* obj_0011c.h -- classes whose methods sit at 0x11C700-0x1200FF */
#ifndef GAME_OBJ_0011C_H
#define GAME_OBJ_0011C_H

#include "common.h"

/*
 * Two small C++ class hierarchies. Each class is named after its vtable
 * (Class_XXXXXXXX has vtable D_XXXXXXXX). Each vtable starts with two
 * zero header words; slot 0 is the deleting destructor. The destructors
 * show the hierarchy: each stores its own vtable, then its base's.
 * Slots whose purpose is unknown are not declared yet, so the vtable types
 * stay incomplete; methods below take only the object, further parameters
 * are unknown.
 */

/* ---- base D_00223510: 3 slots (destructor, slot 1 pure, slot 2) ---- */

typedef struct Class_00223510 Class_00223510;
typedef struct Class_00223530 Class_00223530;

struct Class_00223510 {
    const struct Class_00223510Vtbl *vtbl;  /* 0x00 */
};

/* derives from Class_00223510 */
struct Class_00223530 {
    Class_00223510 base;        /* 0x00 */
    u8             unk_04[0x1C];
    s32            unk_20;      /* 0x20 cleared by slot 2 */
};

void func_0011C7A0(Class_00223510 *obj);  /* slot 2, also in D_002234F0 */
void func_0011FDB0(Class_00223530 *obj);  /* slot 2 */

/* ---- base D_00223650: 6 slots (destructor, slots 1-2 pure, 3-5) ---- */

typedef struct Class_00223650 Class_00223650;
typedef struct Class_00223550 Class_00223550;  /* derives from Class_00223650 */
typedef struct Class_002235B0 Class_002235B0;  /* derives from Class_00223650 */
typedef struct Class_002235D0 Class_002235D0;  /* derives from Class_00223650 */
typedef struct Class_002235F0 Class_002235F0;  /* derives from Class_00223650 */
typedef struct Class_00223610 Class_00223610;  /* derives from Class_00223650 */

s32  func_0011DA60(Class_00223650 *obj);  /* slot 3 */
void func_0011DA50(Class_00223650 *obj);  /* slot 4 */
void func_0011DC40(Class_00223650 *obj);  /* slot 5 */
s32  func_0011E3A0(Class_00223550 *obj);  /* slot 3 */
void func_0011ED50(Class_002235B0 *obj);  /* slot 1 */
void func_0011F000(Class_002235D0 *obj);  /* slot 1 */
s32  func_0011F070(Class_002235F0 *obj);  /* slot 3 */
void func_0011F080(Class_002235F0 *obj);  /* slot 1 */
void func_0011F390(Class_00223610 *obj);  /* slot 1 */

#endif /* GAME_OBJ_0011C_H */
