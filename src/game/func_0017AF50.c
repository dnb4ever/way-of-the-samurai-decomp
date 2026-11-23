/* func_0017AF50.c -- list: end */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_0017AF50

Returns, in *it, the list's end iterator, built by func_0017AF80.

Not byte-exact: retail saves $s0 with the 128-bit prologue no published
EE compiler emits (AGENTS.md, "Known gap").
================
*/
ListIter *func_0017AF50(ListIter *it, List *list)
{
    func_0017AF80(it, &list->head);
    return it;
}

#endif /* NON_MATCHING */
