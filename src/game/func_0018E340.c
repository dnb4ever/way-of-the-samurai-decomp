/* func_0018E340.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_0018E340

Returns the address of the element of the iterator's node.
================
*/
void *func_0018E340(ListIter *it)
{
    return &it->node->value;
}
