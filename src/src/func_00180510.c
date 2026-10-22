extern u8 D_00223C40[];

void *func_00180510(void *arg0, void *arg1) {
    arg1 = ((void * volatile *)arg1)[1];
    *(void **)((u8 *)arg0 + 4) = (void *)D_00223C40;
    *(void **)arg0 = arg1;
    return arg0;
}