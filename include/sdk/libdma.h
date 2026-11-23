/* libdma.h -- libdma: the EE DMA controller's channels */
#ifndef SDK_LIBDMA_H
#define SDK_LIBDMA_H

#include "common.h"

/* DMA channel IDs */
#define SCE_DMA_VIF0        0
#define SCE_DMA_VIF1        1
#define SCE_DMA_GIF         2
#define SCE_DMA_fromIPU     3
#define SCE_DMA_toIPU       4
#define SCE_DMA_fromSIF0    5
#define SCE_DMA_toSIF1      6
#define SCE_DMA_SIF2        7
#define SCE_DMA_fromSPR     8
#define SCE_DMA_toSPR       9

/* Dn_CHCR bits */
#define D_CHCR_STR          0x100   /* channel running */

/* a channel's registers (only as far as used here) */
typedef struct {
    u32 chcr;           /* 0x00 */
    u8  unk_04[0x7C];
} sceDmaChan;

sceDmaChan *sceDmaGetChan(int id);
int         sceDmaDebug(int enable);
u32         sceDmaPause(sceDmaChan *d);
int         sceDmaRestart(sceDmaChan *d, u32 chcr);

#endif /* SDK_LIBDMA_H */
