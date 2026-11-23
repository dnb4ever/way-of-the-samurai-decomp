/* func_0015AEE0.c -- constructor */

#include "game/obj_0015a.h"

/*
================
func_0015AEE0

Runs func_0015A8E0 on obj and returns obj. Any further parameters are
passed through unchanged; their number is unknown.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
#ifdef NON_MATCHING
Obj_0015AEE0 *func_0015AEE0(Obj_0015AEE0 *obj)
{
    func_0015A8E0(obj);
    return obj;
}
#endif
