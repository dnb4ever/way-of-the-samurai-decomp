/* func_00149D30.c -- exchange a member */

#include "game/unsorted.h"

/*
================
func_00149D30

Sets unk_00 to value and returns its old value.
================
*/
s32 func_00149D30(Obj_00149D30 *obj, s32 value)
{
    s32 old;

    old = obj->unk_00;
    obj->unk_00 = value;
    return old;
}
