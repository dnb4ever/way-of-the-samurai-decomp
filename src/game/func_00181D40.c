/* func_00181D40.c -- list iterator: operator++ */

#include "game/linkedlist.h"

/*
================
func_00181D40

Advances the iterator to the next node. Returns it.
================
*/
ListIter *func_00181D40(ListIter *it)
{
    ListNode *node = it->node;

    node = node->next;
    it->node = node;
    return it;
}
