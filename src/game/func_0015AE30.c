/* func_0015AE30.c -- constructor */

#include "game/obj_0015a.h"

#ifdef NON_MATCHING

/*
================
func_0015AE30

Runs func_0015B520 on obj and returns obj. Any further parameters are
passed through unchanged; their number is unknown.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
Obj_0015AE30 *func_0015AE30(Obj_0015AE30 *obj)
{
    func_0015B520(obj);
    return obj;
}
#endif
