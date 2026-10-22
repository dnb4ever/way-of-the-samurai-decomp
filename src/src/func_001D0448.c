extern u8 D_0021D380[];
extern u8 D_0021D390[];
s32 strcmp(void *, void *);

void *func_001D0448(void *obj, s32 arg1, void *arg2) {
    if (arg2 != NULL) {
        if (strcmp(arg2, D_0021D390) != 0) {
            if (strcmp(arg2, D_0021D380) != 0) {
                return NULL;
            }
        }
        *(s32 *)((u8 *)obj + 0x30) = arg1;
        *(void **)((u8 *)obj + 0x34) = arg2;
    }
    return (void *)D_0021D390;
}