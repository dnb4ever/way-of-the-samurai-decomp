/* libvu0.h -- libvu0: vector and matrix arithmetic on VU0 */
#ifndef SDK_LIBVU0_H
#define SDK_LIBVU0_H

#include "common.h"

typedef float sceVu0FVECTOR[4] __attribute__((aligned(16)));
typedef float sceVu0FMATRIX[4][4] __attribute__((aligned(16)));

void func_00105CE0(sceVu0FVECTOR v0, sceVu0FVECTOR v1);  /* v0 = v1 */
void sceVu0CopyMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1);
void sceVu0CopyVectorXYZ(sceVu0FVECTOR v0, sceVu0FVECTOR v1);

#endif /* SDK_LIBVU0_H */
