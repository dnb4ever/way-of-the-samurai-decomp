/* libpkt.h -- libpkt: building GIF and VIF1 DMA packets in memory */
#ifndef SDK_LIBPKT_H
#define SDK_LIBPKT_H

#include "common.h"
#include "sdk/eestruct.h"

/* DMA tag IDs, in the ID field (bits 28-30) of a tag's first word */
#define SCE_DMA_ID_CNT      0x10000000
#define SCE_DMA_ID_REF      0x30000000
#define SCE_DMA_ID_END      0x70000000

/* a ref tag's address field: bits 29-30 (uncached, scratchpad) cleared */
#define SCE_DMA_ADDR_MASK   0x9fffffff

/* a GIF packet: a DMA chain whose data goes to PATH3 */
typedef struct {
    u32  *pCurrent;  /* 0x00 next word to write */
    u128 *pBase;     /* 0x04 start of the packet */
    u128 *pDmaTag;   /* 0x08 open cnt/end tag, its QWC written on close */
    u64  *pGifTag;   /* 0x0C open GIF tag, its NLOOP written on close */
} sceGifPacket;

/* a VIF1 packet: a DMA chain of VIF codes and their data */
typedef struct {
    u32  *pCurrent;  /* 0x00 next word to write */
    u128 *pBase;     /* 0x04 start of the packet */
    u128 *pDmaTag;   /* 0x08 open cnt/end tag, its QWC written on close */
    u32  *pVifCode;  /* 0x0C open UNPACK or DIRECT code */
    u32   numlen;    /* 0x10 open UNPACK: write cycle << 16 | bits per cycle */
    u64  *pGifTag;   /* 0x14 open GIF tag, its NLOOP written on close */
} sceVif1Packet;

void  sceGifPkInit(sceGifPacket *pPacket, u128 *pBase);
void  sceGifPkReset(sceGifPacket *pPacket);
u128 *sceGifPkTerminate(sceGifPacket *pPacket);
void  sceGifPkCnt(sceGifPacket *pPacket, u32 opt1, u32 opt2, u32 flag);
void  sceGifPkRef(sceGifPacket *pPacket, u128 *pRef, u32 size, u32 opt1, u32 opt2, u32 flag);
void  sceGifPkEnd(sceGifPacket *pPacket, u32 opt1, u32 opt2, u32 flag);
u32  *sceGifPkReserve(sceGifPacket *pPacket, u32 count);
void  sceGifPkOpenGifTag(sceGifPacket *pPacket, u128 gifTag);
void  sceGifPkCloseGifTag(sceGifPacket *pPacket);
void  sceGifPkAddGsAD(sceGifPacket *pPacket, u32 addr, u64 data);

void  sceVif1PkInit(sceVif1Packet *pPacket, u128 *pBase);
void  sceVif1PkReset(sceVif1Packet *pPacket);
u128 *sceVif1PkTerminate(sceVif1Packet *pPacket);
u32   sceVif1PkSize(sceVif1Packet *pPacket);
void  sceVif1PkCnt(sceVif1Packet *pPacket, u32 flag);
void  sceVif1PkRef(sceVif1Packet *pPacket, u128 *pRef, u32 size, u32 opt1, u32 opt2, u32 flag);
void  sceVif1PkEnd(sceVif1Packet *pPacket, u32 flag);
void  sceVif1PkOpenUpkCode(sceVif1Packet *pPacket, u16 vuaddr, u32 upkcmd, u32 cl, u32 wl);
void  sceVif1PkCloseUpkCode(sceVif1Packet *pPacket);
void  sceVif1PkOpenDirectCode(sceVif1Packet *pPacket, int stall);
void  sceVif1PkCloseDirectCode(sceVif1Packet *pPacket);
void  sceVif1PkOpenGifTag(sceVif1Packet *pPacket, u128 gifTag);
void  sceVif1PkCloseGifTag(sceVif1Packet *pPacket);
u32  *sceVif1PkReserve(sceVif1Packet *pPacket, u32 count);
void  sceVif1PkAlign(sceVif1Packet *pPacket, u32 bit, u32 pos);
void  sceVif1PkAddCode(sceVif1Packet *pPacket, u32 code);
void  sceVif1PkAddData(sceVif1Packet *pPacket, u32 data);
void  sceVif1PkAddDataN(sceVif1Packet *pPacket, u32 *pData, u32 count);
void  sceVif1PkAddGsData(sceVif1Packet *pPacket, u64 data);
void  sceVif1PkAddUpkData128(sceVif1Packet *pPacket, u128 data);
void  sceVif1PkAddGsAD(sceVif1Packet *pPacket, u32 addr, u64 data);
void  sceVif1PkRefLoadImage(sceVif1Packet *pPacket, u16 bp, u8 psm, u16 bw, u128 *image, u32 size,
                            u32 x, u32 y, u32 w, u32 h);
void  sceVif1PkOpenDirectHLCode(sceVif1Packet *pPacket, int stall);
void  sceVif1PkCloseDirectHLCode(sceVif1Packet *pPacket);
void  sceVif1PkAddUpkData128N(sceVif1Packet *pPacket, u128 *pData, u32 count);
void  sceVif1PkAddGsDataN(sceVif1Packet *pPacket, u64 *pData, u32 count);

/*
================
PkPadQword

Library-internal (name invented): zero-fills from p to the next quadword
boundary and returns the new write position.
================
*/
static inline u32 *PkPadQword(u32 *p)
{
    while ((u32)p & 0xc) {
        *p++ = 0;
    }
    return p;
}

#endif /* SDK_LIBPKT_H */
