/* func_001335B0.c -- constructor */

#include "game/unsorted.h"

#ifdef NON_MATCHING

/*
================
func_001335B0

Runs func_00133420 on obj and returns obj. Any further parameters are
passed through unchanged; their number is unknown.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
Obj_001335B0 *func_001335B0(Obj_001335B0 *obj)
{
    func_00133420(obj);
    return obj;
}
#endif
