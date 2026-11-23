/* eestruct.h -- EE hardware words: GIF tags, VIF codes and GS registers */
#ifndef SDK_EESTRUCT_H
#define SDK_EESTRUCT_H

#include "common.h"

/* GIF tag FLG field (bits 58-59) */
#define SCE_GIF_PACKED      0
#define SCE_GIF_REGLIST     1
#define SCE_GIF_IMAGE       2

/* GIF tag REGS descriptor: address + data pairs */
#define SCE_GIF_PACKED_AD   0xe

/* a GIF tag's first doubleword */
#define SCE_GIF_SET_TAG(nloop, eop, pre, prim, flg, nreg) \
    ((u64)(nloop) | ((u64)(eop) << 15) | ((u64)(pre) << 46) | \
     ((u64)(prim) << 47) | ((u64)(flg) << 58) | ((u64)(nreg) << 60))

/* VIF codes, in bits 24-31 of a VIF code word */
#define SCE_VIF1_STCYCL     0x01000000
#define SCE_VIF1_DIRECT     0x50000000
#define SCE_VIF1_DIRECTHL   0x51000000
#define SCE_VIF1_IRQ        0x80000000  /* interrupt after the code */

/* DIRECTHL: count quadwords to the GIF through PATH2 */
#define SCE_VIF1_SET_DIRECTHL(count, irq) \
    ((u32)(count) | SCE_VIF1_DIRECTHL | ((u32)(irq) << 31))

/* GS registers, as A+D addresses */
#define SCE_GS_BITBLTBUF    0x50
#define SCE_GS_TRXPOS       0x51
#define SCE_GS_TRXREG       0x52
#define SCE_GS_TRXDIR       0x53

#define SCE_GS_SET_BITBLTBUF(sbp, sbw, spsm, dbp, dbw, dpsm) \
    ((u64)(sbp) | ((u64)(sbw) << 16) | ((u64)(spsm) << 24) | \
     ((u64)(dbp) << 32) | ((u64)(dbw) << 48) | ((u64)(dpsm) << 56))
#define SCE_GS_SET_TRXPOS(ssax, ssay, dsax, dsay, dir) \
    ((u64)(ssax) | ((u64)(ssay) << 16) | ((u64)(dsax) << 32) | \
     ((u64)(dsay) << 48) | ((u64)(dir) << 59))
#define SCE_GS_SET_TRXREG(rrw, rrh) \
    ((u64)(rrw) | ((u64)(rrh) << 32))
#define SCE_GS_SET_TRXDIR(xdr) \
    ((u64)(xdr))

#endif /* SDK_EESTRUCT_H */
