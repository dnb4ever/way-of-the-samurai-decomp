/* func_001F02C0.c -- list iterator: operator++ */

#include "game/linkedlist.h"

/*
================
func_001F02C0

Advances the iterator to the next node. Returns it.
================
*/
ListIter *func_001F02C0(ListIter *it)
{
    ListNode *node = it->node;

    node = node->next;
    it->node = node;
    return it;
}
