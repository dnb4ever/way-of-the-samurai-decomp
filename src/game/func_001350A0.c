/* func_001350A0.c -- constructor */

#include "game/unsorted.h"

/*
================
func_001350A0

Runs func_001350D0 on obj and returns obj. Any further parameters are
passed through unchanged; their number is unknown.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
#ifdef NON_MATCHING
Obj_001350A0 *func_001350A0(Obj_001350A0 *obj)
{
    func_001350D0(obj);
    return obj;
}
#endif
