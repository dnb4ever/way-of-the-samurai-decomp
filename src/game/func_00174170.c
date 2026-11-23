/* func_00174170.c -- list: begin */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_00174170

Returns, in *it, an iterator (vtable D_00223B80) on the list's first node: the
C form of a member function that returns the iterator by value.

Not byte-exact: retail loads list->head.next into $a1 before it forms the
vtable address in $v1; from C the two values get the other registers and
the other order.
================
*/
ListIter *func_00174170(ListIter *it, List *list)
{
    it->vtbl = &D_00223B80;
    it->node = list->head.next;
    return it;
}

#endif /* NON_MATCHING */
