#include "context.h"

// matched by autopilot (operator: you)
/* machine draft (m2c), tried by autopilot */
#ifndef M2C_DEFINES
#define M2C_DEFINES
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
#define M2C_BITWISE(type, expr) ((type)(expr))
#define M2C_ERROR(desc) (0)
#define M2C_CARRY 0
#define M2C_OVERFLOW(a) (0)
#endif

M2C_UNK func_0010F9C8();                            /* extern */
M2C_UNK func_0010FB78();                            /* extern */
M2C_UNK func_00117018();                            /* extern */
M2C_UNK func_001177C0();                            /* extern */
M2C_UNK func_00117850();                            /* extern */
M2C_UNK func_001179D0();                            /* extern */

void func_001178C0(void) {
    func_001177C0();
    func_00117850();
    func_0010F9C8();
    func_0010FB78();
    func_001179D0();
    func_00117018();
}
