/* func_0019BB10.c -- list iterator: operator++ */

#include "game/linkedlist.h"

/*
================
func_0019BB10

Advances the iterator to the next node. Returns it.
================
*/
ListIter *func_0019BB10(ListIter *it)
{
    ListNode *node = it->node;

    node = node->next;
    it->node = node;
    return it;
}
