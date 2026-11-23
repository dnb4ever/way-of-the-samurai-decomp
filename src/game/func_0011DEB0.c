/* func_0011DEB0.c -- constructor */

#include "game/unsorted.h"

#ifdef NON_MATCHING

/*
================
func_0011DEB0

Runs func_0011DC90 on obj and returns obj. Any further parameters are
passed through unchanged; their number is unknown.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
Obj_0011DEB0 *func_0011DEB0(Obj_0011DEB0 *obj)
{
    func_0011DC90(obj);
    return obj;
}
#endif
