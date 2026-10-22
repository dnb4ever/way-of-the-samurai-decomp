void *func_00181E00(void **arg0) {
    void *val;
    val = *(void **)arg0;
    val = *(void **)((char *)val + 4);
    *arg0 = val;
    return arg0;
}
