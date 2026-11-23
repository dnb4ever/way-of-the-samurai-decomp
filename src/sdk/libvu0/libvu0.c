/* libvu0.c -- libvu0: vector and matrix arithmetic on VU0 */

#include "sdk/libvu0.h"

#ifdef NON_MATCHING
/*
================
func_00105CE0

Copies the vector v1 to v0. Parked: the copy goes through $a2, a register
named by inline assembly, as in sceVu0CopyMatrix.
================
*/
void func_00105CE0(sceVu0FVECTOR v0, sceVu0FVECTOR v1)
{
    __asm__ volatile (
        "lq $6, 0x0(%1)\n"
        "sq $6, 0x0(%0)\n"
        : : "r" (v0), "r" (v1) : "$6", "memory");
}

/*
================
sceVu0CopyMatrix

Parked: the rows go through $a2-$t1, registers named by inline assembly.
================
*/
void sceVu0CopyMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1)
{
    __asm__ volatile (
        "lq $6, 0x0(%1)\n"
        "lq $7, 0x10(%1)\n"
        "lq $8, 0x20(%1)\n"
        "lq $9, 0x30(%1)\n"
        "sq $6, 0x0(%0)\n"
        "sq $7, 0x10(%0)\n"
        "sq $8, 0x20(%0)\n"
        "sq $9, 0x30(%0)\n"
        : : "r" (m0), "r" (m1) : "$6", "$7", "$8", "$9", "memory");
}
#endif

/*
================
sceVu0CopyVectorXYZ

Copies x, y and z of v1 to v0; w is left alone.
================
*/
void sceVu0CopyVectorXYZ(sceVu0FVECTOR v0, sceVu0FVECTOR v1)
{
    v0[0] = v1[0];
    v0[1] = v1[1];
    v0[2] = v1[2];
}
