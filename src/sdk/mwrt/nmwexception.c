/* nmwexception.c -- Metrowerks C++ runtime: std::exception and the
   terminate/unexpected handlers

   The original is C++; the functions GCC reproduces are written as the C
   their bytes are. */

#include <stdlib.h>
#include "sdk/mwrt.h"

/*
================
func_00100520

std::exception::what.
================
*/
const char *func_00100520(void *self)
{
    return "exception";
}

#ifdef NON_MATCHING
/*
================
unexpected__3stdFv

Calls the unexpected handler. Parked: retail reaches the handler through
$at, as only the Metrowerks compiler does.
================
*/
void unexpected__3stdFv(void)
{
    D_001FD218();
}

/*
================
terminate__3stdFv

Calls the terminate handler. Parked: $at addressing, as unexpected.
================
*/
void terminate__3stdFv(void)
{
    D_001FD210();
}

/*
================
duhandler__3stdFv

The default unexpected handler: terminates. Parked: $at addressing, as
unexpected.
================
*/
void duhandler__3stdFv(void)
{
    D_001FD210();
}
#endif

#ifdef NON_MATCHING
/*
================
func_001009E0

The default terminate handler. Parked: retail calls abort and returns,
where GCC turns the call into a tail jump.
================
*/
void func_001009E0(void)
{
    abort();
}
#endif
