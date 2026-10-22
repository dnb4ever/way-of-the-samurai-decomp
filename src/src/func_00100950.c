typedef void (*func_ptr_t)(void);

extern u8 D_001FD218[];

void func_00100950(void) {
    (*(func_ptr_t *)D_001FD218)();
}

void *func_00100A90(void *arg0, u32 *arg1) {
    u8 *p = (u8 *)arg0;
    u8 c = p[0];
    if (!(c & 1)) {
        *arg1 = c >> 1;
        return p + 1;
    }
    if (!(c & 2)) {
        *arg1 = ((c >> 2) << 8) | p[1];
        return p + 2;
    }
    if (!(c & 4)) {
        *arg1 = p[2] | (((c >> 3) << 16) | (p[1] << 8));
        return p + 3;
    }
    *arg1 = p[3] | ((p[2] << 8) | (((c >> 3) << 24) | (p[1] << 16)));
    return p + 4;
}

void *func_00100B30(void *arg0, s32 *arg1) {
    s8 c = *(s8 *)arg0;
    u8 *p = (u8 *)arg0;
    if (!(c & 1)) {
        *arg1 = c >> 1;
        return p + 1;
    }
    if (!(c & 2)) {
        *arg1 = ((c >> 2) << 8) | p[1];
        return p + 2;
    }
    if (!(c & 4)) {
        *arg1 = p[2] | (((c >> 3) << 16) | (p[1] << 8));
        return p + 3;
    }
    *arg1 = p[3] | ((p[2] << 8) | (((c >> 3) << 24) | (p[1] << 16)));
    return p + 4;
}