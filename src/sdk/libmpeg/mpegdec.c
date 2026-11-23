/* mpegdec.c -- libmpeg: macroblock decoding with the IPU */

#include "sdk/libmpeg.h"

/* IPU registers */
#define IPU_CMD     ((volatile u32 *)0x10002000)  /* volatile: hardware register */
#define IPU_CTRL    ((volatile u32 *)0x10002010)  /* volatile: hardware register */

#define IPU_CTRL_MP1_O  23  /* MPEG-1 bit stream */
#define IPU_CTRL_MP1_M  (1 << IPU_CTRL_MP1_O)

/* what each IPU command (by its top four bits) leaves to wait for */
static int D_001FD390[16] = {
    1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 2, 0, 2, 0, 2, 3,
};

#ifdef NON_MATCHING
/*
================
_ipuSetMPEG1

Switches the IPU between MPEG-1 and MPEG-2 bit streams. Parked: retail
puts the volatile store in the return's delay slot, which 991111 does not
do (a plain store does match: the compiler question, not the source).
================
*/
void _ipuSetMPEG1(int mpeg1)
{
    *IPU_CTRL = (*IPU_CTRL & ~IPU_CTRL_MP1_M) | (mpeg1 << IPU_CTRL_MP1_O);
}
#endif

/*
================
_dualPrimeVector

Derives the dual-prime motion vectors (ISO/IEC 13818-2 7.6.3.6), as
Dual_Prime_Arithmetic in the MPEG Software Simulation Group's mpeg2decode.
================
*/
void _dualPrimeVector(MpegSys *sys, int dmv[2][2], int *dmvector, int mvx, int mvy)
{
    if (sys->pictureStructure == MPEG_FRAME_PICTURE) {
        if (sys->topFieldFirst) {
            /* top field from bottom field */
            dmv[0][0] = ((mvx + (mvx > 0)) >> 1) + dmvector[0];
            dmv[0][1] = ((mvy + (mvy > 0)) >> 1) + dmvector[1] - 1;

            /* bottom field from top field */
            dmv[1][0] = ((3 * mvx + (mvx > 0)) >> 1) + dmvector[0];
            dmv[1][1] = ((3 * mvy + (mvy > 0)) >> 1) + dmvector[1] + 1;
        } else {
            /* top field from bottom field */
            dmv[0][0] = ((3 * mvx + (mvx > 0)) >> 1) + dmvector[0];
            dmv[0][1] = ((3 * mvy + (mvy > 0)) >> 1) + dmvector[1] - 1;

            /* bottom field from top field */
            dmv[1][0] = ((mvx + (mvx > 0)) >> 1) + dmvector[0];
            dmv[1][1] = ((mvy + (mvy > 0)) >> 1) + dmvector[1] + 1;
        }
    } else {
        /* the opposite parity field */
        dmv[0][0] = ((mvx + (mvx > 0)) >> 1) + dmvector[0];
        dmv[0][1] = ((mvy + (mvy > 0)) >> 1) + dmvector[1];

        /* vertical correction for the field parity */
        if (sys->pictureStructure == MPEG_TOP_FIELD) {
            dmv[0][1]--;
        } else {
            dmv[0][1]++;
        }
    }
}

/*
================
_sendIpuCommand
================
*/
void _sendIpuCommand(MpegSys *sys, u32 command)
{
    *IPU_CMD = command;
    sys->unk_818 = D_001FD390[command >> 28];
}
