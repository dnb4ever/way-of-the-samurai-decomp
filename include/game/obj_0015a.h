/* obj_0015a.h -- classes whose methods sit at 0x15A800-0x15B5FF */
#ifndef GAME_OBJ_0015A_H
#define GAME_OBJ_0015A_H

#include "common.h"
#include "sdk/libvu0.h"

/* a VU0 vector: its copy constructors copy it with func_00105CE0 */
typedef struct Obj_0015AF70 {
    sceVu0FVECTOR v;  /* 0x00 x, y, z, w */
} Obj_0015AF70;

Obj_0015AF70 *func_0015ABC0(Obj_0015AF70 *ret, Obj_0015AF70 *vec);
Obj_0015AF70 *func_0015AC00(Obj_0015AF70 *vec);
Obj_0015AF70 *func_0015AE60(Obj_0015AF70 *vec, Obj_0015AF70 *src);
Obj_0015AF70 *func_0015AF10(Obj_0015AF70 *vec, Obj_0015AF70 *src);
Obj_0015AF70 *func_0015AF40(Obj_0015AF70 *vec, Obj_0015AF70 *src);
Obj_0015AF70 *func_0015AF70(Obj_0015AF70 *vec, f32 x, f32 y, f32 z, f32 w);

/* constructors that run another constructor; their objects and further
   parameters are unknown */
typedef struct Obj_0015AE30 Obj_0015AE30;
typedef struct Obj_0015AEE0 Obj_0015AEE0;
typedef struct Obj_0015B4C0 Obj_0015B4C0;

void          func_0015B520(Obj_0015AE30 *obj);
void          func_0015A8E0(Obj_0015AEE0 *obj);
void          func_0015B000(Obj_0015B4C0 *obj);
Obj_0015AE30 *func_0015AE30(Obj_0015AE30 *obj);
Obj_0015AEE0 *func_0015AEE0(Obj_0015AEE0 *obj);
Obj_0015B4C0 *func_0015B4C0(Obj_0015B4C0 *obj);

#endif /* GAME_OBJ_0015A_H */
