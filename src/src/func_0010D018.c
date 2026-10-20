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

s32 func_0010D018(void *arg0) {
    void *temp_2;
    void *temp_2_2;
    void *temp_2_3;
    void *temp_2_4;
    void *temp_2_5;
    void *temp_2_6;
    void *temp_3;

    temp_3 = M2C_FIELD(arg0, void **, 0x40);
    temp_2 = M2C_FIELD(temp_3, void **, 0x1B8);
    if (temp_2 != NULL) {
        M2C_FIELD(temp_2, s32 *, 0x28) = 0;
    }
    temp_2_2 = M2C_FIELD(temp_3, void **, 0x1C8);
    if (temp_2_2 != NULL) {
        M2C_FIELD(temp_2_2, s32 *, 0x28) = 0;
    }
    temp_2_3 = M2C_FIELD(temp_3, void **, 0x1D8);
    if (temp_2_3 != NULL) {
        M2C_FIELD(temp_2_3, s32 *, 0x28) = 0;
    }
    temp_2_4 = M2C_FIELD(temp_3, void **, 0x1BC);
    if (temp_2_4 != NULL) {
        M2C_FIELD(temp_2_4, s32 *, 0x28) = 0;
    }
    temp_2_5 = M2C_FIELD(temp_3, void **, 0x1CC);
    if (temp_2_5 != NULL) {
        M2C_FIELD(temp_2_5, s32 *, 0x28) = 0;
    }
    temp_2_6 = M2C_FIELD(temp_3, void **, 0x1DC);
    if (temp_2_6 != NULL) {
        M2C_FIELD(temp_2_6, s32 *, 0x28) = 0;
    }
    return 1;
}
