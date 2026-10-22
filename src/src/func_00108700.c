void func_00108700(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4) {
    s32 t;
    if (*(s32 *)((u8 *)arg0 + 0x174) == 3) {
        if (*(s32 *)((u8 *)arg0 + 0x178) != 0) {
            ((s32*)arg1)[0] = (arg3 > 0 ? (arg3+1)>>1 : arg3>>1) + ((s32*)arg2)[0];
            ((s32*)arg1)[1] = (arg4 > 0 ? (arg4+1)>>1 : arg4>>1) + ((s32*)arg2)[1] - 1;
            t = arg3 * 3;
            ((s32*)arg1)[2] = ((arg3 > 0 ? t+1 : t) >> 1) + ((s32*)arg2)[0];
            t = arg4 * 3;
            ((s32*)arg1)[3] = ((arg4 > 0 ? t+1 : t) >> 1) + ((s32*)arg2)[1] + 1;
        } else {
            t = arg3 * 3;
            ((s32*)arg1)[0] = ((arg3 > 0 ? t+1 : t) >> 1) + ((s32*)arg2)[0];
            t = arg4 * 3;
            ((s32*)arg1)[1] = ((arg4 > 0 ? t+1 : t) >> 1) + ((s32*)arg2)[1] - 1;
            ((s32*)arg1)[2] = (arg3 > 0 ? (arg3+1)>>1 : arg3>>1) + ((s32*)arg2)[0];
            ((s32*)arg1)[3] = (arg4 > 0 ? (arg4+1)>>1 : arg4>>1) + ((s32*)arg2)[1] + 1;
        }
    } else {
        s32 b;
        ((s32*)arg1)[0] = (arg3 > 0 ? (arg3+1)>>1 : arg3>>1) + ((s32*)arg2)[0];
        b = (arg4 > 0 ? (arg4+1)>>1 : arg4>>1) + ((s32*)arg2)[1];
        ((s32*)arg1)[1] = b;
        if (*(s32 *)((u8 *)arg0 + 0x174) == 1) {
            ((s32*)arg1)[1] = b - 1;
        } else {
            ((s32*)arg1)[1] = b + 1;
        }
    }
}