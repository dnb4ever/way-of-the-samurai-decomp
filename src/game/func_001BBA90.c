/* func_001BBA90.c -- list iterator: element */

#include "game/linkedlist.h"

/*
================
func_001BBA90

Returns the element of the iterator's node, stored after its links.
================
*/
void *func_001BBA90(ListIter *it)
{
    return it->node + 1;
}
