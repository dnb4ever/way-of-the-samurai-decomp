/* func_00134970.c -- list iterator: construct */

#include "game/listiter.h"

/*
================
func_00134970

Constructs an iterator of vtable D_00223670 at node and returns it.

Parked: retail loads the vtable address into $v0, stores both members
through $a0 and copies $a0 to $v0 last; the stock compilers copy $a0
to $v0 first and store through it, in either statement order. Only a
volatile store reproduces it, which is not source.
Also tried: a base-constructor helper storing the vtable and returning
this, an initialiser helper for the node, a null-checked this, the node
as void *, a const pointer or a one-member struct; none gives retail's
registers at -O2, -Os, -O1 or -O3.
================
*/
#ifdef NON_MATCHING
ListIter *func_00134970(ListIter *it, ListNode *node)
{
    it->vtbl = &D_00223670;
    it->node = node;
    return it;
}
#endif
