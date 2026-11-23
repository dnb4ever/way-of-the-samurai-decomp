/* vu0.h -- the libvu0 types and functions the game code here calls */
#ifndef GAME_VU0_H
#define GAME_VU0_H

#include "common.h"

/*
 * Sony libvu0 declarations, kept here until include/sdk has a libvu0.h;
 * the names are libvu0's own (config/symbols_debug.txt).
 */
typedef float sceVu0FVECTOR[4] __attribute__((aligned(16)));
typedef float sceVu0FMATRIX[4][4] __attribute__((aligned(16)));

void func_00105CE0(sceVu0FVECTOR v0, sceVu0FVECTOR v1);  /* v0 = v1 */
void sceVu0CopyMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1);

#endif /* GAME_VU0_H */
