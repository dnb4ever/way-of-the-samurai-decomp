/* libgraph.c -- libgraph: GS set-up */

#include "sdk/libgraph.h"

/* interlaced, NTSC, field mode */
static sceGsGParam D_001FD230 = { 1, 2, 1, 3 };

/*
================
func_00102820

Returns the GS parameters.
================
*/
sceGsGParam *func_00102820(void)
{
    return &D_001FD230;
}
