/* libmpeg.c -- libmpeg: the decoder's settings and state queries */

#include "sdk/libmpeg.h"

/*
================
func_0010CE60
================
*/
int func_0010CE60(sceMpeg *mp)
{
    return 1;
}

/*
================
sceMpegSetDecodeMode

Sets how many I, P and B pictures to decode.
================
*/
void sceMpegSetDecodeMode(sceMpeg *mp, int ni, int np, int nb)
{
    MpegSys *sys = mp->sys;

    sys->ni = ni;
    sys->np = np;
    sys->nb = nb;
}

/*
================
sceMpegGetDecodeMode
================
*/
void sceMpegGetDecodeMode(sceMpeg *mp, int *ni, int *np, int *nb)
{
    MpegSys *sys = mp->sys;

    *ni = sys->ni;
    *np = sys->np;
    *nb = sys->nb;
}

/*
================
func_0010CFA8
================
*/
int func_0010CFA8(sceMpeg *mp)
{
    return mp->sys->unk_00;
}

/*
================
sceMpegIsRefBuffEmpty
================
*/
int sceMpegIsRefBuffEmpty(sceMpeg *mp)
{
    return mp->sys->unk_04 == 0;
}

/*
================
sceMpegClearRefBuff
================
*/
int sceMpegClearRefBuff(sceMpeg *mp)
{
    MpegSys *sys = mp->sys;

    if (sys->refSlot[0].unk_00 != NULL) {
        sys->refSlot[0].unk_00->unk_28 = 0;
    }
    if (sys->refSlot[1].unk_00 != NULL) {
        sys->refSlot[1].unk_00->unk_28 = 0;
    }
    if (sys->refSlot[2].unk_00 != NULL) {
        sys->refSlot[2].unk_00->unk_28 = 0;
    }
    if (sys->refSlot[0].unk_04 != NULL) {
        sys->refSlot[0].unk_04->unk_28 = 0;
    }
    if (sys->refSlot[1].unk_04 != NULL) {
        sys->refSlot[1].unk_04->unk_28 = 0;
    }
    if (sys->refSlot[2].unk_04 != NULL) {
        sys->refSlot[2].unk_04->unk_28 = 0;
    }
    return 1;
}

/*
================
sceMpegSetDefaultPtsGap
================
*/
void sceMpegSetDefaultPtsGap(sceMpeg *mp, s64 gap)
{
    MpegSys *sys = mp->sys;

    sys->ptsGapSet = 1;
    sys->ptsGap = gap;
}

/*
================
sceMpegResetDefaultPtsGap
================
*/
void sceMpegResetDefaultPtsGap(sceMpeg *mp)
{
    MpegSys *sys = mp->sys;

    sys->ptsGapSet = 0;
    sys->ptsGap = 0;
}

/*
================
func_0010D138
================
*/
void func_0010D138(sceMpeg *mp, u32 addr)
{
    mp->sys->unk_D8 = addr;
}

/*
================
func_0010D148
================
*/
int func_0010D148(sceMpeg *mp)
{
    return mp->sys->unk_CC;
}

/*
================
func_0010D158
================
*/
int func_0010D158(sceMpeg *mp)
{
    return mp->sys->unk_D0;
}

/*
================
func_0010D168
================
*/
void *func_0010D168(sceMpeg *mp)
{
    return mp->sys->unk_B4;
}

/*
================
func_0010D178

Same body as func_0010D168.
================
*/
void *func_0010D178(sceMpeg *mp)
{
    return mp->sys->unk_B4;
}

/*
================
sceSetBrokenLink

Sets the broken-link flag and returns the previous one.
================
*/
int sceSetBrokenLink(sceMpeg *mp, int brokenLink)
{
    MpegSys *sys = mp->sys;
    int      old = sys->brokenLink;

    sys->brokenLink = brokenLink;
    return old;
}

/*
================
sceSetPtm
================
*/
void sceSetPtm(sceMpeg *mp, s64 ptm)
{
    MpegSys *sys = mp->sys;

    sys->ptm = ptm;
    sys->ptmSet = 1;
}
