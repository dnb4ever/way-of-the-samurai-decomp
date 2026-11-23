/* func_001858A0.c -- list iterator: operator++ */

#include "game/linkedlist.h"

/*
================
func_001858A0

Advances the iterator to the next node. Returns it.
================
*/
ListIter *func_001858A0(ListIter *it)
{
    ListNode *node = it->node;

    node = node->next;
    it->node = node;
    return it;
}
