/* func_001803C0.c -- list iterator: construct */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_001803C0

Constructs an iterator of one list instance (vtable D_00223C40) on node.
Returns it.

Not byte-exact: retail stores the vtable through $a0 and copies $a0 to
$v0 between the two stores; from C the copy comes first and both stores
go through $v0, node first.
================
*/
ListIter *func_001803C0(ListIter *it, ListNode *node)
{
    it->vtable = &D_00223C40;
    it->node = node;
    return it;
}

#endif /* NON_MATCHING */
