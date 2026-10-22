void stream_put_s64_split(void **arg0, s64 arg1) {
    s32 *p = (s32 *)*arg0;
    *p++ = (s32)arg1;
    *p = (s32)(arg1 >> 32);
    *arg0 = (void *)(p + 1);
}