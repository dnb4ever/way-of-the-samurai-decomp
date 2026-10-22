void *func_001BD2E0(void **arg0) {
    void *val;
    val = *(void **)arg0;
    val = *(void **)((u8 *)val + 4);
    *arg0 = val;
    return arg0;
}
