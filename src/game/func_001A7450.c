/* func_001A7450.c -- set the word at 0x4 */

#include "game/upper.h"

/*
================
func_001A7450

Sets the word at 0x4.
================
*/
void func_001A7450(Obj_001A73D0 *obj, s32 value)
{
    obj->unk_04 = value;
}
