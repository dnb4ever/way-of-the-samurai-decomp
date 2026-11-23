/* obj_00131f98.h -- the object driven by the handler table D_00202C90 */
#ifndef GAME_OBJ_00131F98_H
#define GAME_OBJ_00131F98_H

#include "common.h"
#include "game/vu0.h"

/*
 * func_00131F98(obj, index, a, b) calls handler D_00202C90[index](obj, a, b)
 * and returns its result. Most handlers ask func_00132280 for a request
 * (a group, 1 or 2, and a code) and return whether it was accepted.
 */
typedef struct Obj_00131F98 {
    u8             unk_000[0x388];
    sceVu0FVECTOR *unk_388;  /* 0x388 a matrix */
} Obj_00131F98;

void func_00124080(Obj_00131F98 *obj);
s32  func_00132280(Obj_00131F98 *obj, s32 group, s32 code, s32 arg);

/* the object at D_008DCB70; both functions pass owner, obj and arg on to
   func_001B8630 and return 0 */
typedef struct Obj_001B83E0 Obj_001B83E0;
s32  func_001B83C0(Obj_001B83E0 *owner, Obj_00131F98 *obj, s32 arg);
s32  func_001B83E0(Obj_001B83E0 *owner, Obj_00131F98 *obj, s32 arg);
void func_001B8630(Obj_001B83E0 *owner, Obj_00131F98 *obj, s32 arg);

/* handlers in D_00202C90 */
s32 func_0012C270(Obj_00131F98 *obj);
s32 func_0012C650(Obj_00131F98 *obj);
s32 func_0012C670(Obj_00131F98 *obj);
s32 func_0012C810(Obj_00131F98 *obj);
s32 func_0012D360(Obj_00131F98 *obj);
s32 func_0012D420(Obj_00131F98 *obj);
s32 func_0012F7C0(Obj_00131F98 *obj);
s32 func_00131EC0(Obj_00131F98 *obj, sceVu0FVECTOR *matrix);

#endif /* GAME_OBJ_00131F98_H */
