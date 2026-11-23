/* func_0011C7B0.c -- set unk_84 */

#include "game/unsorted.h"

#ifdef NON_MATCHING

/*
================
func_0011C7B0

Sets unk_84 to func_001F71D0(arg).

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
void func_0011C7B0(Obj_0011C7B0 *obj, s32 arg)
{
    obj->unk_84 = func_001F71D0(arg);
}
#endif
