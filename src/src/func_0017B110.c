extern u8 D_00223CB0[];
typedef struct { void *f0; void *f4; } ST_func_0017A8E0;
ST_func_0017A8E0 *func_0017B110(ST_func_0017A8E0 *arg0, void *arg1) {
    arg1 = ((void * volatile *)arg1)[1];
    arg0->f4 = (void *)D_00223CB0;
    arg0->f0 = arg1;
    return arg0;
}