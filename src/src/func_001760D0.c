void *func_001760D0(void **arg0) {
    void *val;
    val = *(void **)arg0;
    val = *(void **)val;
    *arg0 = (void **)val;
    return arg0;
}
