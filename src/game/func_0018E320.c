/* func_0018E320.c -- list iterator: operator++ */

#include "game/linkedlist.h"

/*
================
func_0018E320

Advances the iterator to the next node. Returns it.
================
*/
ListIter *func_0018E320(ListIter *it)
{
    ListNode *node = it->node;

    node = node->next;
    it->node = node;
    return it;
}
