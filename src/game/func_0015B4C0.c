/* func_0015B4C0.c -- constructor */

#include "game/obj_0015a.h"

#ifdef NON_MATCHING

/*
================
func_0015B4C0

Runs func_0015B000 on obj and returns obj. Any further parameters are
passed through unchanged; their number is unknown.

Parked: retail saves $s0 with sq (AGENTS.md, "Known gap").
================
*/
Obj_0015B4C0 *func_0015B4C0(Obj_0015B4C0 *obj)
{
    func_0015B000(obj);
    return obj;
}
#endif
