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

s32 func_00132280(s32, M2C_UNK, M2C_UNK, M2C_UNK);  /* extern */
M2C_UNK func_001B83E0(s32, s32, M2C_UNK);           /* extern */

s32 func_0012D420(s32 arg0) {
    func_001B83E0(*(s32 *)0x8DCB70, arg0, 1);
    func_00124080(arg0);
    return func_00132280(arg0, 2, 0x6B, 0) != 0;
}
