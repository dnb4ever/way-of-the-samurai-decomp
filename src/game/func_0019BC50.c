/* func_0019BC50.c -- list iterator: construct */

#include "game/linkedlist.h"

#ifdef NON_MATCHING

/*
================
func_0019BC50

Constructs an iterator of one list instance (vtable D_00223E10) on node.
Returns it.

Not byte-exact: retail stores the vtable through $a0 and copies $a0 to
$v0 between the two stores; from C the copy comes first and both stores
go through $v0, node first.
================
*/
ListIter *func_0019BC50(ListIter *it, ListNode *node)
{
    it->vtbl = &D_00223E10;
    it->node = node;
    return it;
}

#endif /* NON_MATCHING */
