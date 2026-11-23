/* func_00179420.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_00179420

Returns the address of the element of the iterator's node.
================
*/
void *func_00179420(ListIter *it)
{
    return &it->node->value;
}
