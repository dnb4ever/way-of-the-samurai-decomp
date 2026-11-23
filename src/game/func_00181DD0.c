/* func_00181DD0.c -- list iterator: operator++ */

#include "game/linkedlist.h"

/*
================
func_00181DD0

Advances the iterator to the next node. Returns it.
================
*/
ListIter *func_00181DD0(ListIter *it)
{
    ListNode *node = it->node;

    node = node->next;
    it->node = node;
    return it;
}
