/* func_001F2CC0.c -- list iterator: operator-- */

#include "game/linkedlist.h"

/*
================
func_001F2CC0

Moves the iterator back to the previous node. Returns it.
================
*/
ListIter *func_001F2CC0(ListIter *it)
{
    ListNode *node = it->node;

    node = node->prev;
    it->node = node;
    return it;
}
