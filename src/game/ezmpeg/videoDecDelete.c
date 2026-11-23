/* videoDecDelete.c -- delete a video decoder */

#include "game/ezmpeg.h"

#ifdef NON_MATCHING

/*
================
videoDecDelete

Releases the decoder's ViBuf (func_001ECFF0), then its libmpeg decoder
(func_0010CE60). Returns 1.

Not byte-exact: retail saves $s0 with the 128-bit prologue no published
EE compiler emits (AGENTS.md, "Known gap").
================
*/
s32 videoDecDelete(VideoDec *vd)
{
    func_001ECFF0(&vd->vibuf);
    func_0010CE60(&vd->mpeg);
    return 1;
}

#endif /* NON_MATCHING */
