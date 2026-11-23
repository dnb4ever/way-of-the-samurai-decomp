/* func_0015AE60.c -- vector: copy constructor */

#include "game/obj_0015a.h"

#ifdef NON_MATCHING

/*
================
func_0015AE60

Constructs vec as a copy of src and returns it.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
Obj_0015AF70 *func_0015AE60(Obj_0015AF70 *vec, Obj_0015AF70 *src)
{
    func_00105CE0(vec->v, src->v);
    return vec;
}
#endif
