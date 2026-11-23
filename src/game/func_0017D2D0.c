/* func_0017D2D0.c -- list: begin */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_0017D2D0

Returns, in *it, an iterator (vtable D_00223B40) on the list's first node: the
C form of a member function that returns the iterator by value.

Not byte-exact: retail loads list->head.next into $a1 before it forms the
vtable address in $v1; from C the two values get the other registers and
the other order.
================
*/
ListIter *func_0017D2D0(ListIter *it, List *list)
{
    it->vtable = &D_00223B40;
    it->node = list->head.next;
    return it;
}

#endif /* NON_MATCHING */
