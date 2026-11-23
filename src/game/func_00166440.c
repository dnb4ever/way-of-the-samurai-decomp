/* func_00166440.c -- list iterator: construct */

#include "game/listiter.h"

/*
================
func_00166440

Constructs an iterator of vtable D_00223B40 at node and returns it.

Parked: retail loads the vtable address into $v0, stores both members
through $a0 and copies $a0 to $v0 last; the stock compilers copy $a0
to $v0 first and store through it, in either statement order. Only a
volatile store reproduces it, which is not source.
================
*/
#ifdef NON_MATCHING
ListIter *func_00166440(ListIter *it, ListNode *node)
{
    it->vtbl = &D_00223B40;
    it->node = node;
    return it;
}
#endif
