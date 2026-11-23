/* func_001B69A0.c -- set the float at 0xD18 */

#include "game/upper.h"

/*
================
func_001B69A0

Sets the float at 0xD18. Returns 1.
================
*/
s32 func_001B69A0(Obj_001B6990 *obj, f32 value)
{
    obj->unk_D18 = value;
    return 1;
}
