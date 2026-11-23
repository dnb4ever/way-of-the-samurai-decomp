/* func_0018E350.c -- list iterator: construct */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_0018E350

Constructs an iterator of one list instance (vtable D_00223D70) on node.
Returns it.

Not byte-exact: retail stores the vtable through $a0 and copies $a0 to
$v0 between the two stores; from C the copy comes first and both stores
go through $v0, node first.
================
*/
ListIter *func_0018E350(ListIter *it, ListNode *node)
{
    it->vtable = &D_00223D70;
    it->node = node;
    return it;
}

#endif /* NON_MATCHING */
