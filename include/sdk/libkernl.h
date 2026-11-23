/* libkernl.h -- libkernl internals: the console TTY over DECI2 */
#ifndef SDK_LIBKERNL_H
#define SDK_LIBKERNL_H

#include "common.h"

int  sceTtyInit(void);
int  sceTtyWrite(const char *buf, int len);
int  sceTtyRead(char *buf, int len);

void Exit(int status);

#endif /* SDK_LIBKERNL_H */
