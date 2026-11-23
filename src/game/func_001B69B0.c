/* func_001B69B0.c -- set the float at 0xD14 */

#include "game/upper.h"

/*
================
func_001B69B0

Sets the float at 0xD14. Returns 1.
================
*/
s32 func_001B69B0(Unk001B6990 *obj, f32 value)
{
    obj->unk_D14 = value;
    return 1;
}
