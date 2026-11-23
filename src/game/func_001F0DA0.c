/* func_001F0DA0.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_001F0DA0

Returns the address of the element of the iterator's node.
================
*/
void *func_001F0DA0(ListIter *it)
{
    return &it->node->value;
}
