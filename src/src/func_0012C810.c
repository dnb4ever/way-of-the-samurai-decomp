s32 func_00132280();

s32 func_0012C910(s32 arg0, s32 arg1) {
    if (arg1) {
        if (func_00132280(arg0, 1, 0x18, 0)) return 1;
    } else {
        if (func_00132280(arg0, 1, 0x17, 0)) return 1;
    }
    return 0;
}

s32 func_0012C8A0(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0xCC0) += 1;
    if (arg1) {
        if (func_00132280(arg0, 1, 0x1A, 0)) return 1;
    } else {
        if (func_00132280(arg0, 1, 0x19, 0)) return 1;
    }
    return 0;
}

s32 func_0012C840(s32 arg0, s32 arg1) {
    if (arg1) {
        if (func_00132280(arg0, 1, 0x1D, 0)) return 1;
    } else {
        if (func_00132280(arg0, 1, 0x1C, 0)) return 1;
    }
    return 0;
}

s32 func_0012C810(s32 arg0) {
    return func_00132280(arg0, 1, 0x1B, 0) != 0;
}
