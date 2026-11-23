/* func_001B9B10.c -- list iterator: operator++ */

#include "game/linkedlist.h"

/*
================
func_001B9B10

Advances the iterator to the next node. Returns it.
================
*/
ListIter *func_001B9B10(ListIter *it)
{
    ListNode *node = it->node;

    node = node->next;
    it->node = node;
    return it;
}
