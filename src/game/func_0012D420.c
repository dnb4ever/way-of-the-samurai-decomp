/* func_0012D420.c -- handler: func_001B83E0, then request 2/0x6B */

#include "game/obj_00131f98.h"
#include "game/obj_008dcb00.h"

#ifdef NON_MATCHING

/*
================
func_0012D420

Handler of D_00202C90: passes obj to func_001B83E0 of the object at
D_008DCB00.unk_70, runs func_00124080 on it, then returns whether request
0x6B of group 2 is accepted.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
Retail also reads D_008DCB00.unk_70 with an $at address macro where the
stock compilers split the address into $v0.
================
*/
s32 func_0012D420(Obj_00131F98 *obj)
{
    func_001B83E0(D_008DCB00.unk_70, obj, 1);
    func_00124080(obj);
    return func_00132280(obj, 2, 0x6B, 0) != 0;
}
#endif
