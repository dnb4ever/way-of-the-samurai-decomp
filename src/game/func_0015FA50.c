/* func_0015FA50.c -- list iterator: step back */

#include "game/listiter.h"

/*
================
func_0015FA50

Moves the iterator to the previous node (--it) and returns it.
Slot 3 (prev) of the iterator vtable D_00223A10.

Parked: retail keeps the loaded node in $v1 for both loads; the stock
compilers at -O2 load the first into $a1. Only merging the two values
into one local reproduces it, which is not source.
(ee-gcc2.96 and 2.9-991111 at -Os allocate like retail.)
================
*/
#ifdef NON_MATCHING
ListIter *func_0015FA50(ListIter *it)
{
    it->node = it->node->prev;
    return it;
}
#endif
