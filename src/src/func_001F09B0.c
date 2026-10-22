extern u8 D_00224010[];
typedef struct { void *f0; void *f4; } ST_func_0017A8E0;
ST_func_0017A8E0 *func_001F09B0(ST_func_0017A8E0 *arg0, void *arg1) {
    arg1 = ((void * volatile *)arg1)[1];
    arg0->f4 = (void *)D_00224010;
    arg0->f0 = arg1;
    return arg0;
}