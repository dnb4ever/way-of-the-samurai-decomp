/* func_0015AF70.c -- vector: construct from components */

#include "game/obj_0015a.h"

/*
================
func_0015AF70

Constructs vec from its four components and returns it.
================
*/
Obj_0015AF70 *func_0015AF70(Obj_0015AF70 *vec, f32 x, f32 y, f32 z, f32 w)
{
    vec->v[0] = x;
    vec->v[1] = y;
    vec->v[2] = z;
    vec->v[3] = w;
    return vec;
}
