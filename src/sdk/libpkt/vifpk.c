/* vifpk.c -- libpkt: VIF1 packets, DMA chains for PATH1 and PATH2 */

#include "sdk/eestruct.h"
#include "sdk/libpkt.h"

/*
================
sceVif1PkInit
================
*/
void sceVif1PkInit(sceVif1Packet *pPacket, u128 *pBase)
{
    pPacket->pDmaTag = NULL;
    pPacket->pBase = pBase;
    pPacket->pCurrent = (u32 *)pBase;
}

/*
================
sceVif1PkReset
================
*/
void sceVif1PkReset(sceVif1Packet *pPacket)
{
    pPacket->pCurrent = (u32 *)pPacket->pBase;
    pPacket->pDmaTag = NULL;
}

/*
================
sceVif1PkTerminate

Pads the packet to a quadword and writes the open DMA tag's QWC.
================
*/
u128 *sceVif1PkTerminate(sceVif1Packet *pPacket)
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
sceVif1PkSize

Returns the packet's length in quadwords.
================
*/
u32 sceVif1PkSize(sceVif1Packet *pPacket)
{
    return ((u32)pPacket->pCurrent - (u32)pPacket->pBase) >> 4;
}

/*
================
sceVif1PkCnt

Opens a cnt tag. Its upper half is the first VIF code of the data.
================
*/
void sceVif1PkCnt(sceVif1Packet *pPacket, u32 flag)
{
    pPacket->pDmaTag = sceVif1PkTerminate(pPacket);
    *pPacket->pCurrent++ = flag | SCE_DMA_ID_CNT;
    *pPacket->pCurrent++ = 0;
    pPacket->pVifCode = NULL;
}

/*
================
sceVif1PkRef

Adds a ref tag: size quadwords sent from pRef, preceded by the VIF codes
opt1 and opt2.
================
*/
void sceVif1PkRef(sceVif1Packet *pPacket, u128 *pRef, u32 size, u32 opt1, u32 opt2, u32 flag)
{
    sceVif1PkTerminate(pPacket);
    *pPacket->pCurrent++ = flag | SCE_DMA_ID_REF | size;
    *pPacket->pCurrent++ = (u32)pRef & SCE_DMA_ADDR_MASK;
    *pPacket->pCurrent++ = opt1;
    *pPacket->pCurrent++ = opt2;
}

/*
================
sceVif1PkEnd
================
*/
void sceVif1PkEnd(sceVif1Packet *pPacket, u32 flag)
{
    pPacket->pDmaTag = sceVif1PkTerminate(pPacket);
    *pPacket->pCurrent++ = flag | SCE_DMA_ID_END;
    *pPacket->pCurrent++ = 0;
    pPacket->pVifCode = NULL;
}

#ifdef NON_MATCHING
/*
================
sceVif1PkOpenUpkCode

Adds STCYCL and UNPACK codes; the UNPACK's NUM is written on close.
Parked: the STCYCL word lands in the other of v0/v1, and the two arms of
the cycle test build the length's upper half differently.
================
*/
void sceVif1PkOpenUpkCode(sceVif1Packet *pPacket, u16 vuaddr, u32 upkcmd, u32 cl, u32 wl)
{
    u32 *p = pPacket->pCurrent;
    u32  bits = (32 >> (upkcmd & 3)) * (((upkcmd >> 2) & 3) + 1);
    u32  c = cl ? cl : 256;
    u32  w = wl ? wl : 256;

    *p++ = SCE_VIF1_STCYCL | (wl << 8) | cl;
    *p = (upkcmd << 24) | vuaddr;
    pPacket->pVifCode = p;
    pPacket->pCurrent = p + 1;
    if (w <= c) {
        w = 1;
    } else {
        bits *= c;
    }
    pPacket->numlen = (w << 16) | bits;
}
#endif

/*
================
sceVif1PkCloseUpkCode
================
*/
void sceVif1PkCloseUpkCode(sceVif1Packet *pPacket)
{
    u32 *last = pPacket->pCurrent - 1;
    u32 *code = pPacket->pVifCode;
    u32  bits = pPacket->numlen & 0xffff;
    u32  n = ((last - code) * 32 + bits - 1) / bits;

    pPacket->pVifCode = NULL;
    *code += (pPacket->numlen & 0xffff0000) * n;
}

/*
================
sceVif1PkOpenDirectCode
================
*/
void sceVif1PkOpenDirectCode(sceVif1Packet *pPacket, int stall)
{
    u32 *p;

    sceVif1PkAlign(pPacket, 2, 3);
    p = pPacket->pCurrent;
    *p = stall ? SCE_VIF1_IRQ | SCE_VIF1_DIRECT : SCE_VIF1_DIRECT;
    pPacket->pVifCode = p;
    pPacket->pCurrent = p + 1;
}

/*
================
sceVif1PkCloseDirectCode

Writes the DIRECT code's quadword count.
================
*/
void sceVif1PkCloseDirectCode(sceVif1Packet *pPacket)
{
    u32 *last = pPacket->pCurrent - 1;
    u32 *code = pPacket->pVifCode;

    pPacket->pVifCode = NULL;
    *code += (u32)(last - code) / 4;
}

/*
================
sceVif1PkOpenGifTag
================
*/
void sceVif1PkOpenGifTag(sceVif1Packet *pPacket, u128 gifTag)
{
    u32 *p = pPacket->pCurrent;

    *(u128 *)p = gifTag;
    pPacket->pGifTag = (u64 *)p;
    pPacket->pCurrent = p + 4;
}

/*
================
sceVif1PkCloseGifTag

Writes the loop count of the data added since the open GIF tag into its
NLOOP field, then pads to a quadword.
================
*/
void sceVif1PkCloseGifTag(sceVif1Packet *pPacket)
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
sceVif1PkReserve
================
*/
u32 *sceVif1PkReserve(sceVif1Packet *pPacket, u32 count)
{
    u32 *p = pPacket->pCurrent;

    pPacket->pCurrent = p + count;
    return p;
}

/*
================
sceVif1PkAlign

Zero-fills up to word pos of the next 2^(bit+2)-byte block.
================
*/
void sceVif1PkAlign(sceVif1Packet *pPacket, u32 bit, u32 pos)
{
    u32 mask = 0xffffffff >> (32 - ((bit + 2) & 31));
    u32 addr = ((u32)pPacket->pCurrent & ~mask) + pos * 4;

    if (addr < (u32)pPacket->pCurrent) {
        addr += mask + 1;
    }
    while ((u32)pPacket->pCurrent < addr) {
        *pPacket->pCurrent++ = 0;
    }
}

/*
================
sceVif1PkAddCode
================
*/
void sceVif1PkAddCode(sceVif1Packet *pPacket, u32 code)
{
    *pPacket->pCurrent++ = code;
}

/*
================
sceVif1PkAddData
================
*/
void sceVif1PkAddData(sceVif1Packet *pPacket, u32 data)
{
    *pPacket->pCurrent++ = data;
}

/*
================
sceVif1PkAddDataN
================
*/
void sceVif1PkAddDataN(sceVif1Packet *pPacket, u32 *pData, u32 count)
{
    while (count--) {
        *pPacket->pCurrent++ = *pData++;
    }
}

/*
================
sceVif1PkAddGsData
================
*/
void sceVif1PkAddGsData(sceVif1Packet *pPacket, u64 data)
{
    *pPacket->pCurrent++ = data;
    *pPacket->pCurrent++ = data >> 32;
}

/*
================
sceVif1PkAddUpkData128
================
*/
void sceVif1PkAddUpkData128(sceVif1Packet *pPacket, u128 data)
{
    u64 lo = ((u64 *)&data)[0];
    u64 hi = ((u64 *)&data)[1];

    *pPacket->pCurrent++ = lo;
    *pPacket->pCurrent++ = lo >> 32;
    *pPacket->pCurrent++ = hi;
    *pPacket->pCurrent++ = hi >> 32;
}

/*
================
sceVif1PkAddGsAD

Adds an A+D pair: data for the GS register addr.
================
*/
void sceVif1PkAddGsAD(sceVif1Packet *pPacket, u32 addr, u64 data)
{
    *pPacket->pCurrent++ = data;
    *pPacket->pCurrent++ = data >> 32;
    *pPacket->pCurrent++ = addr;
    *pPacket->pCurrent++ = 0;
}

/*
================
sceVif1PkRefLoadImage

Adds a transfer of an image from main memory to GS local memory: the
transfer registers through PATH2, then the image itself by reference, in
IMAGE-mode GIF packets of at most 0x7fff quadwords.
================
*/
void sceVif1PkRefLoadImage(sceVif1Packet *pPacket, u16 bp, u8 psm, u16 bw, u128 *image, u32 size,
                           u32 x, u32 y, u32 w, u32 h)
{
    u64  giftag[2] = { SCE_GIF_SET_TAG(0, 0, 0, 0, SCE_GIF_PACKED, 1), SCE_GIF_PACKED_AD };
    u32  n;
    u64 *p;

    sceVif1PkCnt(pPacket, 0);
    sceVif1PkOpenDirectHLCode(pPacket, 0);
    sceVif1PkOpenGifTag(pPacket, *(u128 *)giftag);
    sceVif1PkAddGsAD(pPacket, SCE_GS_BITBLTBUF, SCE_GS_SET_BITBLTBUF(0, 0, 0, bp, bw, psm));
    sceVif1PkAddGsAD(pPacket, SCE_GS_TRXPOS, SCE_GS_SET_TRXPOS(0, 0, x, y, 0));
    sceVif1PkAddGsAD(pPacket, SCE_GS_TRXREG, SCE_GS_SET_TRXREG(w, h));
    sceVif1PkAddGsAD(pPacket, SCE_GS_TRXDIR, SCE_GS_SET_TRXDIR(0));
    sceVif1PkCloseGifTag(pPacket);
    sceVif1PkCloseDirectHLCode(pPacket);

    while (size != 0) {
        n = size > 0x7fff ? 0x7fff : size;
        sceVif1PkCnt(pPacket, 0);
        sceVif1PkAlign(pPacket, 2, 3);
        sceVif1PkAddCode(pPacket, SCE_VIF1_SET_DIRECTHL(1, 0));
        p = (u64 *)sceVif1PkReserve(pPacket, 4);
        p[0] = SCE_GIF_SET_TAG(n, n == size ? 1 : 0, 0, 0, SCE_GIF_IMAGE, 0);
        p[1] = 0;
        sceVif1PkRef(pPacket, image, n, 0, SCE_VIF1_SET_DIRECTHL(n, 0), 0);
        size -= n;
        image += n;
    }
}

/*
================
sceVif1PkOpenDirectHLCode
================
*/
void sceVif1PkOpenDirectHLCode(sceVif1Packet *pPacket, int stall)
{
    u32 *p;

    sceVif1PkAlign(pPacket, 2, 3);
    p = pPacket->pCurrent;
    *p = stall ? SCE_VIF1_IRQ | SCE_VIF1_DIRECTHL : SCE_VIF1_DIRECTHL;
    pPacket->pVifCode = p;
    pPacket->pCurrent = p + 1;
}

/*
================
sceVif1PkCloseDirectHLCode
================
*/
void sceVif1PkCloseDirectHLCode(sceVif1Packet *pPacket)
{
    u32 *last = pPacket->pCurrent - 1;
    u32 *code = pPacket->pVifCode;

    pPacket->pVifCode = NULL;
    *code += (u32)(last - code) / 4;
}

/*
================
sceVif1PkAddUpkData128N
================
*/
void sceVif1PkAddUpkData128N(sceVif1Packet *pPacket, u128 *pData, u32 count)
{
    u64 *d = (u64 *)pData;

    while (count--) {
        *pPacket->pCurrent++ = d[0];
        *pPacket->pCurrent++ = d[0] >> 32;
        *pPacket->pCurrent++ = d[1];
        *pPacket->pCurrent++ = d[1] >> 32;
        d += 2;
    }
}

/*
================
sceVif1PkAddGsDataN
================
*/
void sceVif1PkAddGsDataN(sceVif1Packet *pPacket, u64 *pData, u32 count)
{
    while (count--) {
        *pPacket->pCurrent++ = *pData;
        *pPacket->pCurrent++ = *pData >> 32;
        pData++;
    }
}
