/* func_001B69C0.c -- set the float at 0xD10 */

#include "game/upper.h"

/*
================
func_001B69C0

Sets the float at 0xD10. Returns 1.
================
*/
s32 func_001B69C0(Unk001B6990 *obj, f32 value)
{
    obj->unk_D10 = value;
    return 1;
}
