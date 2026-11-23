/* func_0012C270.c -- handler: request 2/0x6B */

#include "game/obj_00131f98.h"

/*
================
func_0012C270

Handler of D_00202C90: runs func_00124080 on obj, then returns whether
request 0x6B of group 2 is accepted.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
#ifdef NON_MATCHING
s32 func_0012C270(Obj_00131F98 *obj)
{
    func_00124080(obj);
    return func_00132280(obj, 2, 0x6B, 0) != 0;
}
#endif
