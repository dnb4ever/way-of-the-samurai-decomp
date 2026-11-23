/* eekernel.h -- libkernl: EE kernel calls and their library wrappers */
#ifndef SDK_EEKERNEL_H
#define SDK_EEKERNEL_H

#include "common.h"

/* Deci2Call function numbers */
#define DECI2_OPEN          1
#define DECI2_CLOSE         2
#define DECI2_REQSEND       3
#define DECI2_POLL          4
#define DECI2_EXRECV        (-5)
#define DECI2_EXSEND        (-6)
#define DECI2_EXREQSEND     (-7)
#define DECI2_EXLOCK        (-8)
#define DECI2_EXUNLOCK      (-9)
#define DECI2_KPUTS         16

/* KSEG1: the uncached view of main memory */
#define UNCACHED_SEG        0x20000000

int Deci2Call(int func, u32 *param);

#endif /* SDK_EEKERNEL_H */
