/* runtime.c -- Metrowerks runtime: double comparisons for the compiler

   Metrowerks compiled this object; _dpfgt's bytes are also what GCC
   produces. */

#include "sdk/mwrt.h"

/*
================
_dpfgt
================
*/
int _dpfgt(double a, double b)
{
    return dpcmp(a, b) > 0;
}

#ifdef NON_MATCHING
/*
================
_dpflt

Parked: retail tests the sign with slt against $zero, Metrowerks code
generation; GCC shifts the sign bit down.
================
*/
int _dpflt(double a, double b)
{
    return dpcmp(a, b) < 0;
}

/*
================
_dpfeq

Parked: retail compares with an xor against $zero before sltiu, which
only the Metrowerks compiler emits.
================
*/
int _dpfeq(double a, double b)
{
    return dpcmp(a, b) == 0;
}
#endif
