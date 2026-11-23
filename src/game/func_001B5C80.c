/* func_001B5C80.c -- set the word at 0x150 of the part at 0x4 */

#include "game/upper.h"

/*
================
func_001B5C80

Slot 3 of vtable D_00223E90: stores value at 0x150 of the object at 0x4.
Returns value.
================
*/
s32 func_001B5C80(Obj_00223E90 *obj, s32 value)
{
    obj->unk_04->unk_150 = value;
    return value;
}
