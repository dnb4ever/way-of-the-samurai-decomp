extern u8 vtable_00223CE0[];

typedef struct { s32 f0; s32 f4; } ObjA;

void *func_0017A8E0(ObjA *arg0, ObjA *arg1) {
    arg0->f4 = (s32)vtable_00223CE0;
    arg0->f0 = arg1->f4;
    return arg0;
}

s32 D_00224A10;
s32 D_00224A24;

void func_0018E400(s32 arg0) {
    D_00224A10 = arg0;
}

void func_0018E410(s32 arg0) {
    D_00224A24 = arg0;
}

s32 func_001898D0(void);

s32 func_0018D880(void *arg0) {
    if ((func_001898D0() & 0xFF) == 0xB) {
        *(s8 *)((u8 *)arg0 + 9) = 0;
    }
    return 1;
}
