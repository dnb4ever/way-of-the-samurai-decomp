/* func_0015AF40.c -- vector: copy constructor */

#include "game/obj_0015a.h"

/*
================
func_0015AF40

Constructs vec as a copy of src and returns it.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
#ifdef NON_MATCHING
Obj_0015AF70 *func_0015AF40(Obj_0015AF70 *vec, Obj_0015AF70 *src)
{
    func_00105CE0(vec->v, src->v);
    return vec;
}
#endif
