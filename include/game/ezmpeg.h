/* ezmpeg.h -- Sony's ezmpeg sample player, as linked into the game */
#ifndef GAME_EZMPEG_H
#define GAME_EZMPEG_H

#include "common.h"

/*
 * The sample was compiled as C++: its functions carry mangled names in
 * other games' symbols (videoDecDelete__FP8VideoDec), which also give the
 * class names below. Only the members the functions here use are known.
 */

/*
 * libmpeg's decoder and callback data (Sony SDK). Their layout belongs to
 * the SDK headers; only sceMpeg's size is needed here: a VideoDec embeds
 * it, and the ViBuf follows it at 0x48.
 */
typedef struct sceMpeg {
    u8 unk_00[0x48];
} sceMpeg;

typedef struct sceMpegCbData sceMpegCbData;

typedef struct ReadBuf ReadBuf;

typedef struct ViBuf {
    u8 unk_00[0x60];            /* up to VideoDec's next known member */
} ViBuf;

typedef struct VideoDec {
    sceMpeg mpeg;
    ViBuf   vibuf;
    s32     unk_A8;
} VideoDec;

typedef struct VoBuf {
    u8  unk_00[0x8];
    s32 unk_08;
    s32 unk_0C;
} VoBuf;

extern s32 D_00224AC0;
extern s32 D_00224AD4;

/* Sony SDK functions the sample calls (libkernl, libmpeg) */
s32 RotateThreadReadyQueue(s32 priority);
void *sceMpegAddStrCallback(sceMpeg *mp, s32 strType, s32 ch,
                            s32 (*callback)(sceMpeg *mp, sceMpegCbData *cbdata, void *data),
                            void *data);
s32 func_0010CE60(sceMpeg *mp);

/* functions, by address */
void func_001EB3D0(void);
void func_001EB940(char *message);
void defMain(void *arg);
void func_001EC7D0(ReadBuf *buf);
s32 func_001ECFF0(ViBuf *vibuf);
s32 func_001EDD10(sceMpeg *mp, sceMpegCbData *cbdata, void *data);
s32 func_001EE180(VideoDec *vd);
s32 videoDecDelete(VideoDec *vd);
s32 videoDecSetStream(VideoDec *vd, s32 strType, s32 ch,
                      s32 (*callback)(sceMpeg *mp, sceMpegCbData *cbdata, void *data),
                      void *data);
void func_001EE4C0(VoBuf *buf);
void func_001EE4D0(VoBuf *buf);

#endif /* GAME_EZMPEG_H */
