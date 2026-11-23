/* mwrt.h -- the Metrowerks C/C++ runtime the program links */
#ifndef SDK_MWRT_H
#define SDK_MWRT_H

#include "common.h"

/* soft-float double comparison: <0, 0 or >0 */
int dpcmp(double a, double b);

int _dpfgt(double a, double b);
int _dpflt(double a, double b);
int _dpfeq(double a, double b);

/* the C++ library's exception::what() overrides and allocation */
const char *func_00100520(void *self);   /* std::exception::what */
void       *func_00100530(u32 size);     /* operator new[] */
const char *func_00100620(void *self);   /* std::bad_alloc::what */
void       *func_00100630(u32 size);     /* operator new */
void        func_001009E0(void);         /* the default terminate handler */
const char *func_001021E0(void *self);   /* std::bad_exception::what */

/* the installed terminate and unexpected handlers */
extern void (*D_001FD210)(void);
extern void (*D_001FD218)(void);

/* std::unexpected, std::terminate and their handler hooks */
void unexpected__3stdFv(void);
void terminate__3stdFv(void);
void duhandler__3stdFv(void);

#endif /* SDK_MWRT_H */
