/* func_00190980.c -- word at 0x20 */

#include "game/upper.h"

/*
================
func_00190980

Slot 4 of vtable D_00223DA0: returns the word at 0x20 (slot 3 stores it,
clamped).
================
*/
s32 func_00190980(Unk00223DA0 *obj)
{
    return obj->unk_20;
}
