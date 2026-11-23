/* func_00175B70.c -- compare the value at 0x24: not greater */

#include "game/upper.h"

/*
================
func_00175B70

Returns whether a's value at 0x24 is less than or equal to b's.
================
*/
s32 func_00175B70(Obj_00175B60 *a, Obj_00175B60 *b)
{
    return a->unk_24 <= b->unk_24;
}
