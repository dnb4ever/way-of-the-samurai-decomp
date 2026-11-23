/* func_00186040.c -- list iterator: operator-- */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_00186040

Moves the iterator back to the previous node. Returns it.

Not byte-exact: retail loads the node into $v1 and the next one over it;
from C the node gets $a1. Reading the node into a local and stepping
that local reproduces retail, but only by reusing one variable for two
values, so the honest form is kept here.
================
*/
ListIter *func_00186040(ListIter *it)
{
    it->node = it->node->prev;
    return it;
}

#endif /* NON_MATCHING */
