/* func_00182F10.c -- list: end */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_00182F10

Returns, in *it, the list's end iterator, built by func_0017F500.

Not byte-exact: retail saves $s0 with the 128-bit prologue no published
EE compiler emits (AGENTS.md, "Known gap").
================
*/
ListIter *func_00182F10(ListIter *it, List *list)
{
    func_0017F500(it, &list->head);
    return it;
}

#endif /* NON_MATCHING */
