extern u8 D_00223D70[];
typedef struct { void *f0; void *f4; } ST_func_0018E3C0;
ST_func_0018E3C0 *func_0018E3C0(ST_func_0018E3C0 *arg0, void *arg1) {
    arg1 = ((void * volatile *)arg1)[1];
    arg0->f4 = (void *)D_00223D70;
    arg0->f0 = arg1;
    return arg0;
}
