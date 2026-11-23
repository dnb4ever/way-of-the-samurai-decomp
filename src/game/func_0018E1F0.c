/* func_0018E1F0.c -- list iterator: operator++ */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_0018E1F0

Advances the iterator to the next node. Returns it.

Not byte-exact: retail loads the node into $v1 and the next one over it;
from C the node gets $a1. Reading the node into a local and stepping
that local reproduces retail, but only by reusing one variable for two
values, so the honest form is kept here.
================
*/
ListIter *func_0018E1F0(ListIter *it)
{
    it->node = it->node->next;
    return it;
}

#endif /* NON_MATCHING */
