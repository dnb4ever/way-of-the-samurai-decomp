/* libvu0.c -- libvu0: vector and matrix arithmetic on VU0 */

#include "sdk/libvu0.h"

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
