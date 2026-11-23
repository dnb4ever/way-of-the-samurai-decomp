/* func_00165B00.c -- list iterator: first element */

#include "game/listiter.h"

/*
================
func_00165B00

Constructs an iterator of vtable D_00223B40 at the first node of list
(the node after the sentinel) and returns it.

Parked: retail loads the node into $a1 and the vtable address into $v1
and stores through $a0; the stock compilers load the vtable address into
$v0 and copy $a0 to $v0 after the first store.
================
*/
#ifdef NON_MATCHING
ListIter *func_00165B00(ListIter *it, List *list)
{
    it->vtbl = &D_00223B40;
    it->node = list->head.next;
    return it;
}
#endif
