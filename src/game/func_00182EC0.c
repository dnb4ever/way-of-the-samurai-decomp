/* func_00182EC0.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_00182EC0

Returns the 16-byte aligned element of the iterator's node.
================
*/
void *func_00182EC0(ListIter *it)
{
    ListNodeQ *node = (ListNodeQ *)it->node;

    return &node->unk_10;
}
