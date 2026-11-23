/* func_0019CFE0.c -- call func_001B43C0 and func_001B5240 */

#include "game/upper.h"

#ifdef NON_MATCHING

/*
================
func_0019CFE0

Calls func_001B43C0, then func_001B5240.

Not byte-exact: retail calls func_001B5240 with jal and returns after it.
Both stock compilers at -O2 turn that last call into a tail jump; 2.96 at
-O1 emits the retail code, so the original unit's options or compiler
differ from the -O2 game units.
================
*/
void func_0019CFE0(void)
{
    func_001B43C0();
    func_001B5240();
}

#endif /* NON_MATCHING */
