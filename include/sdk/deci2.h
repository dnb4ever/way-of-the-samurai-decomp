/* deci2.h -- DECI2 protocol access from the EE (libkernl) */
#ifndef SDK_DECI2_H
#define SDK_DECI2_H

#include "common.h"

int sceDeci2Open(u16 protocol, void *opt, void (*handler)(int event, int param, void *opt));
int sceDeci2Close(int s);
int sceDeci2ReqSend(int s, char dest);
void sceDeci2Poll(int s);
int sceDeci2ExRecv(int s, void *buf, u16 len);
int sceDeci2ExSend(int s, void *buf, u16 len);
int sceDeci2ExReqSend(int s, char dest);
int sceDeci2ExLock(int s);
int sceDeci2ExUnLock(int s);
int kputs(char *s);

#endif /* SDK_DECI2_H */
