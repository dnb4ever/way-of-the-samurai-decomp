/* sifdev.h -- IOP memory and module loading over the SIF */
#ifndef SDK_SIFDEV_H
#define SDK_SIFDEV_H

#include "common.h"

int func_00115D30(void *addr);
int func_00115DA8(void *addr);

int func_00116080(void *addr, int argLen, const char *args, int *result);
int sceSifLoadModuleBuffer(void *addr, int argLen, const char *args);
int sceSifLoadStartModuleBuffer(void *addr, int argLen, const char *args, int *result);

int func_00116690(const char *path, int argLen, const char *args, int *result, int unk);
int sceSifLoadModule(const char *path, int argLen, const char *args);
int sceSifLoadStartModule(const char *path, int argLen, const char *args, int *result);

#endif /* SDK_SIFDEV_H */
