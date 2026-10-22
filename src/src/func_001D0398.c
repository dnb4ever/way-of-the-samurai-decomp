extern u8 D_0021D380[];
extern u8 D_0021D390[];
s32 strcmp(void *, void *);

s32 func_001D0398(void *arg0, s32 (*arg1)(void *)) {
    s32 n;
    u8 *p;
    s32 result = 0;
    void **node = (void **)((u8 *)arg0 + 0x1D8);
    if (node != NULL) {
        do {
            n = ((s32 *)node)[1];
            n--;
            p = (u8 *)node[2];
            if (n >= 0) {
                do {
                    if (*(s16 *)(p + 0xC) != 0) {
                        result |= arg1(p);
                    }
                    n--;
                    p += 0x58;
                } while (n >= 0);
            }
            node = (void **)node[0];
        } while (node != NULL);
    }
    return result;
}

void *func_001D0448(void *obj, s32 arg1, void *arg2) {
    if (arg2 != NULL) {
        if (strcmp(arg2, D_0021D390) != 0 && strcmp(arg2, D_0021D380) != 0) {
            return NULL;
        }
        *(void *volatile *)((u8 *)obj + 0x34) = arg2;
        *(volatile s32 *)((u8 *)obj + 0x30) = arg1;
    }
    return (void *)D_0021D390;
}