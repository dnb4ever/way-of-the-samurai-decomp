/* func_00190C10.c -- first word below n */

#include "game/upper.h"

/*
================
func_00190C10

Returns whether the object's first word, unsigned, is below n.
================
*/
s32 func_00190C10(Unk00190C10 *obj, u32 n)
{
    return obj->unk_00 < n;
}
