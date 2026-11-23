/* func_00190950.c -- list iterator: operator-- */

#include "game/linkedlist.h"

/*
================
func_00190950

Moves the iterator back to the previous node. Returns it.
================
*/
ListIter *func_00190950(ListIter *it)
{
    ListNode *node = it->node;

    node = node->prev;
    it->node = node;
    return it;
}
