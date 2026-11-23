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

int  DIntr(void);
int  EIntr(void);

int  _iEnableIntc(int ch);
int  _iDisableIntc(int ch);
int  _iEnableDmac(int ch);
int  _iDisableDmac(int ch);

int  iEnableIntc(int ch);
int  iDisableIntc(int ch);
int  iEnableDmac(int ch);
int  iDisableDmac(int ch);

/* data cache maintenance over [start, end], by 64-byte lines */
void _sceSDC(u32 start, u32 end);
void _sceIDC(u32 start, u32 end);
void iSyncDCache(void *start, void *end);
void iInvalidDCache(void *start, void *end);

#endif /* SDK_EEKERNEL_H */
