/* gifpk.c -- libpkt: GIF packets, DMA chains for PATH3 */

#include "sdk/libpkt.h"

/*
================
sceGifPkInit
================
*/
void sceGifPkInit(sceGifPacket *pPacket, u128 *pBase)
{
    pPacket->pDmaTag = NULL;
    pPacket->pBase = pBase;
    pPacket->pCurrent = (u32 *)pBase;
}

/*
================
sceGifPkReset
================
*/
void sceGifPkReset(sceGifPacket *pPacket)
{
    pPacket->pCurrent = (u32 *)pPacket->pBase;
    pPacket->pDmaTag = NULL;
}

/*
================
sceGifPkTerminate

Pads the packet to a quadword and writes the open DMA tag's QWC.
================
*/
u128 *sceGifPkTerminate(sceGifPacket *pPacket)
{
    u32  *p = PkPadQword(pPacket->pCurrent);
    u128 *tag = pPacket->pDmaTag;

    if (tag != NULL) {
        u32 qwc = (u128 *)p - tag - 1;

        *(u32 *)tag += qwc;
    }
    pPacket->pCurrent = p;
    pPacket->pDmaTag = NULL;
    return (u128 *)p;
}

/*
================
sceGifPkCnt

Opens a cnt tag: the data that follows, up to the next tag.
================
*/
void sceGifPkCnt(sceGifPacket *pPacket, u32 opt1, u32 opt2, u32 flag)
{
    pPacket->pDmaTag = sceGifPkTerminate(pPacket);
    *pPacket->pCurrent++ = flag | SCE_DMA_ID_CNT;
    *pPacket->pCurrent++ = 0;
    *pPacket->pCurrent++ = opt1;
    *pPacket->pCurrent++ = opt2;
}

/*
================
sceGifPkRef

Adds a ref tag: size quadwords sent from pRef.
================
*/
void sceGifPkRef(sceGifPacket *pPacket, u128 *pRef, u32 size, u32 opt1, u32 opt2, u32 flag)
{
    sceGifPkTerminate(pPacket);
    *pPacket->pCurrent++ = flag | SCE_DMA_ID_REF | size;
    *pPacket->pCurrent++ = (u32)pRef & SCE_DMA_ADDR_MASK;
    *pPacket->pCurrent++ = opt1;
    *pPacket->pCurrent++ = opt2;
}

/*
================
sceGifPkEnd

Opens an end tag: the data that follows ends the chain.
================
*/
void sceGifPkEnd(sceGifPacket *pPacket, u32 opt1, u32 opt2, u32 flag)
{
    pPacket->pDmaTag = sceGifPkTerminate(pPacket);
    *pPacket->pCurrent++ = flag | SCE_DMA_ID_END;
    *pPacket->pCurrent++ = 0;
    *pPacket->pCurrent++ = opt1;
    *pPacket->pCurrent++ = opt2;
}

/*
================
sceGifPkReserve

Skips count words and returns where they start.
================
*/
u32 *sceGifPkReserve(sceGifPacket *pPacket, u32 count)
{
    u32 *p = pPacket->pCurrent;

    pPacket->pCurrent = p + count;
    return p;
}

/*
================
sceGifPkOpenGifTag
================
*/
void sceGifPkOpenGifTag(sceGifPacket *pPacket, u128 gifTag)
{
    u32 *p = pPacket->pCurrent;

    *(u128 *)p = gifTag;
    pPacket->pGifTag = (u64 *)p;
    pPacket->pCurrent = p + 4;
}

/*
================
sceGifPkCloseGifTag

Writes the loop count of the data added since the open GIF tag into its
NLOOP field, then pads to a quadword.
================
*/
void sceGifPkCloseGifTag(sceGifPacket *pPacket)
{
    u64 *gifTag = pPacket->pGifTag;
    u32 *p = pPacket->pCurrent;
    u64  tag = *gifTag;
    u32  n = ((u64 *)p - gifTag) - 2;
    u64  flg = (tag >> 58) & 3;
    u32  nreg;

    if (flg != SCE_GIF_REGLIST) {
        n >>= 1;
    }
    if (flg != SCE_GIF_IMAGE) {
        nreg = tag >> 60;
        if (nreg == 0) {
            nreg = 16;
        }
        n = (n + nreg - 1) / nreg;
    }
    pPacket->pGifTag = NULL;
    *gifTag = tag + n;
    pPacket->pCurrent = PkPadQword(p);
}

/*
================
sceGifPkAddGsAD

Adds an A+D pair: data for the GS register addr.
================
*/
void sceGifPkAddGsAD(sceGifPacket *pPacket, u32 addr, u64 data)
{
    u64 *p = (u64 *)pPacket->pCurrent;

    *p++ = data;
    *p++ = addr;
    pPacket->pCurrent = (u32 *)p;
}
