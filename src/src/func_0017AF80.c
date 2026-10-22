extern u8 vtable_00223CE0[];
extern u8 D_00223CB0[];

typedef struct { void *f0; void *f4; } Pair;

Pair *func_0017A860(Pair *arg0, void *arg1) {
    volatile Pair *p = arg0;
    p->f4 = (void *)vtable_00223CE0;
    p->f0 = arg1;
    return arg0;
}

Pair *func_0017AF80(Pair *arg0, void *arg1) {
    volatile Pair *p = arg0;
    p->f4 = (void *)D_00223CB0;
    p->f0 = arg1;
    return arg0;
}

extern s32 func_00172AC0();
s32 func_0017CF40(void *a0, s32 *a1) {
    return func_00172AC0(a0, *a1);
}

typedef struct Node Node;
struct Node { Node *f0; Node *f4; s32 f8; };

extern void *func_00100630(s32);
extern void func_0017D010(void *, void *);

Pair *func_0017CF50(Pair *arg0, Node *arg1, Node **arg2, s32 *arg3) {
    volatile Pair *p;
    Node *n = (Node *)func_00100630(0x48);
    if (n != NULL) {
        n->f8 = *arg3;
        func_0017D010((u8 *)n + 0xC, arg3 + 1);
    }
    n->f0 = (*arg2)->f0;
    n->f4 = *arg2;
    n->f0->f4 = n;
    n->f4->f0 = n;
    arg1->f8 += 1;
    p = arg0;
    p->f4 = (void *)D_00223CB0;
    p->f0 = n;
    return arg0;
}