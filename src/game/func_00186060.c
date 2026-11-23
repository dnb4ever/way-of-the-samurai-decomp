/* func_00186060.c -- list iterator: operator++ */

#include "game/linkedlist.h"

/*
================
func_00186060

Advances the iterator to the next node. Returns it.
================
*/
ListIter *func_00186060(ListIter *it)
{
    ListNode *node = it->node;

    node = node->next;
    it->node = node;
    return it;
}
