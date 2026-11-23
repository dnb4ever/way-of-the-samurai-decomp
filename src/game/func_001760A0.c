/* func_001760A0.c -- list iterator: operator-- */

#include "game/linkedlist.h"

/*
================
func_001760A0

Moves the iterator back to the previous node. Returns it.
================
*/
ListIter *func_001760A0(ListIter *it)
{
    ListNode *node = it->node;

    node = node->prev;
    it->node = node;
    return it;
}
