/* func_001F09B0.c -- list: begin */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_001F09B0

Returns, in *it, an iterator (vtable D_00224010) on the list's first node: the
C form of a member function that returns the iterator by value.

Not byte-exact: retail loads list->head.next into $a1 before it forms the
vtable address in $v1; from C the two values get the other registers and
the other order.
================
*/
ListIter *func_001F09B0(ListIter *it, List *list)
{
    it->vtable = &D_00224010;
    it->node = list->head.next;
    return it;
}

#endif /* NON_MATCHING */
