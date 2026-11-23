/* func_0018E210.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_0018E210

Returns the address of the element of the iterator's node.
================
*/
void *func_0018E210(ListIter *it)
{
    return &it->node->value;
}
