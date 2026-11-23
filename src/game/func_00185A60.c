/* func_00185A60.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_00185A60

Returns the address of the element of the iterator's node.
================
*/
void *func_00185A60(ListIter *it)
{
    return &it->node->value;
}
