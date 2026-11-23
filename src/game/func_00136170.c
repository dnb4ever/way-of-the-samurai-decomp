/* func_00136170.c -- list iterator: step forward */

#include "game/listiter.h"

/*
================
func_00136170

Moves the iterator to the next node (++it) and returns it.
Slot 2 (next) of the iterator vtable D_00223670.

Parked: retail keeps the loaded node in $v1 for both loads; the stock
compilers at -O2 load the first into $a1. Only merging the two values
into one local reproduces it, which is not source.
(ee-gcc2.96 and 2.9-991111 at -Os allocate like retail.)
================
*/
#ifdef NON_MATCHING
ListIter *func_00136170(ListIter *it)
{
    it->node = it->node->next;
    return it;
}
#endif
