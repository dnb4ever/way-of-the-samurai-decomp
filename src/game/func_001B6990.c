/* func_001B6990.c -- set the float at 0xD1C */

#include "game/upper.h"

/*
================
func_001B6990

Sets the float at 0xD1C. Returns 1.
================
*/
s32 func_001B6990(Unk001B6990 *obj, f32 value)
{
    obj->unk_D1C = value;
    return 1;
}
