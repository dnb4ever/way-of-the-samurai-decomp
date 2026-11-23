/* func_00183030.c -- list: begin */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_00183030

Returns, in *it, the list's begin iterator, built by func_0017F630.

Not byte-exact: retail saves $s0 with the 128-bit prologue no published
EE compiler emits (AGENTS.md, "Known gap").
================
*/
ListIter *func_00183030(ListIter *it, List *list)
{
    func_0017F630(it, list);
    return it;
}

#endif /* NON_MATCHING */
