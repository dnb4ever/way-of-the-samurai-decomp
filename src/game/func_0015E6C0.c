/* func_0015E6C0.c -- exchange a member */

#include "game/unsorted.h"

/*
================
func_0015E6C0

Sets unk_1C to value and returns its old value.
================
*/
s32 func_0015E6C0(Obj_0015E6C0 *obj, s32 value)
{
    s32 old;

    old = obj->unk_1C;
    obj->unk_1C = value;
    return old;
}
