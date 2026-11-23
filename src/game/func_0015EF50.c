/* func_0015EF50.c -- list iterator: element address */

#include "game/listiter.h"

/*
================
func_0015EF50

Returns the address of the element at the iterator's node.
Slot 1 (get) of the iterator vtable D_00223A40.
================
*/
void *func_0015EF50(ListIter *it)
{
    return &it->node->value;
}
