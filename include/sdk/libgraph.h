/* libgraph.h -- libgraph: GS set-up and display control */
#ifndef SDK_LIBGRAPH_H
#define SDK_LIBGRAPH_H

#include "common.h"

/* the GS mode sceGsResetGraph set up */
typedef struct {
    s16   sceGsInterMode;       /* 0x00 */
    s16   sceGsOutMode;         /* 0x02 */
    s16   sceGsFFMode;          /* 0x04 */
    s16   sceGsVersion;         /* 0x06 */
    int (*sceGsVSCfunc)(int);   /* 0x08 */
    int   sceGsVSCid;           /* 0x0C */
} sceGsGParam;

sceGsGParam *func_00102820(void);

#endif /* SDK_LIBGRAPH_H */
