/* libmpeg.h -- libmpeg: MPEG-2 video decoding on the IPU */
#ifndef SDK_LIBMPEG_H
#define SDK_LIBMPEG_H

#include "common.h"

/* a decoded or reference picture (fields only as far as used here) */
typedef struct {
    u8  unk_00[0x28];
    int unk_28;         /* 0x28 cleared by sceMpegClearRefBuff */
} MpegPicture;

/* the pictures one reference slot can hold */
typedef struct {
    MpegPicture *unk_00;
    MpegPicture *unk_04;
    MpegPicture *unk_08;
    MpegPicture *unk_0C;
} MpegRefSlot;

/* a callback registered with sceMpegAddCallback */
typedef struct {
    int  (*func)(void *mp, void *cbdata, void *anyData);
    void *data;
} MpegCallback;

/* the decoder's own state, which sceMpeg.sys points to */
typedef struct {
    int          unk_00;            /* 0x00 set when the stream has ended */
    int          unk_04;            /* 0x04 reference pictures held */
    int          unk_08;
    MpegCallback callback[12];      /* 0x0C indexed by callback type */
    int          unk_6C;
    int          ptsGapSet;         /* 0x70 sceMpegSetDefaultPtsGap */
    int          unk_74;
    s64          ptsGap;            /* 0x78 */
    u8           unk_80[0x14];
    int          ni;                /* 0x94 decode modes, sceMpegSetDecodeMode */
    int          np;                /* 0x98 */
    int          nb;                /* 0x9C */
    u8           unk_A0[0x14];
    u8           unk_B4[0x18];      /* 0xB4 handed out whole by two accessors */
    int          unk_CC;
    int          unk_D0;
    int          unk_D4;
    u32          unk_D8;            /* 0xD8 output buffer address */
    u8           unk_DC[0x0C];
    int          brokenLink;        /* 0xE8 sceSetBrokenLink */
    int          unk_EC;
    s64          ptm;               /* 0xF0 sceSetPtm */
    int          ptmSet;            /* 0xF8 */
    u8           unk_FC[0xBC];
    MpegRefSlot  refSlot[3];        /* 0x1B8 */
} MpegSys;

typedef struct {
    int      width;         /* 0x00 */
    int      height;        /* 0x04 */
    int      frameCount;    /* 0x08 */
    s64      pts;           /* 0x10 */
    s64      dts;           /* 0x18 */
    u64      flags;         /* 0x20 */
    s64      pts2nd;        /* 0x28 */
    s64      dts2nd;        /* 0x30 */
    u64      flags2nd;      /* 0x38 */
    MpegSys *sys;           /* 0x40 */
} sceMpeg;

void sceMpegSetDecodeMode(sceMpeg *mp, int ni, int np, int nb);
void sceMpegGetDecodeMode(sceMpeg *mp, int *ni, int *np, int *nb);
int  sceMpegIsRefBuffEmpty(sceMpeg *mp);
int  sceMpegClearRefBuff(sceMpeg *mp);
void sceMpegSetDefaultPtsGap(sceMpeg *mp, s64 gap);
void sceMpegResetDefaultPtsGap(sceMpeg *mp);
int  sceSetBrokenLink(sceMpeg *mp, int brokenLink);
void sceSetPtm(sceMpeg *mp, s64 ptm);

/* the decoder's linear allocator */
typedef struct {
    u8 *base;       /* 0x00 */
    int size;       /* 0x04 */
    u8 *pCurrent;   /* 0x08 next free byte */
    u8 *pDynamic;   /* 0x0C start of the area _alalcFree releases */
} Alalc;

void _alalcInit(Alalc *a, u8 *base, int size);
void _alalcSetDynamic(Alalc *a);
void _alalcFree(Alalc *a);
u8  *func_0010D1E8(MpegSys *sys, Alalc *a, int size, int align);
int  _alalcRest(Alalc *a);

/* the bit reader over the decoder's input ring buffer */
typedef struct {
    u64  buf;       /* 0x00 the next bits, most significant first */
    u8  *start;     /* 0x08 where reading started */
    u8  *pRead;     /* 0x0C next byte to load into buf */
    u32  nbits;     /* 0x10 valid bits in buf */
    s64  pos;       /* 0x18 bits consumed since start */
    u8  *ringBase;  /* 0x20 the ring buffer */
    u8  *ringEnd;   /* 0x24 */
    int  ringSize;  /* 0x28 */
} SysBit;

void  _sysbitInit(SysBit *sb, u8 *start, u8 *ringBase, int ringSize);
int   func_0010BBD8(SysBit *sb, int n);
void  func_0010BBF8(SysBit *sb, int n);
u32   _sysbitGet(SysBit *sb, int n);
u32   _sysbitMarker(SysBit *sb);
void  _sysbitJump(SysBit *sb, int nbytes);
u8   *_sysbitPtr(SysBit *sb, int bitpos);

/* reports a decoder error through the error callback, or prints it */
void _Error(MpegSys *sys, char *msg);

#endif /* SDK_LIBMPEG_H */
