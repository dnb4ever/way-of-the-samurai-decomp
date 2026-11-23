/* func_00134960.c -- list iterator: element address */

#include "game/listiter.h"

/*
================
func_00134960

Returns the address of the element at the iterator's node.
Slot 1 (get) of the iterator vtable D_00223670.
================
*/
void *func_00134960(ListIter *it)
{
    return &it->node->value;
}
