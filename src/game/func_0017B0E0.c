/* func_0017B0E0.c -- list: begin */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_0017B0E0

Returns, in *it, the list's begin iterator, built by func_0017B110.

Not byte-exact: retail saves $s0 with the 128-bit prologue no published
EE compiler emits (AGENTS.md, "Known gap").
================
*/
ListIter *func_0017B0E0(ListIter *it, List *list)
{
    func_0017B110(it, list);
    return it;
}

#endif /* NON_MATCHING */
