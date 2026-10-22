extern u8 D_00223D10[];
typedef struct { void *f0; void *f4; } S00179400;
S00179400 *func_00179400(S00179400 *arg0, void *arg1) {
    volatile S00179400 *p = arg0;
    p->f4 = (void *)D_00223D10;
    p->f0 = arg1;
    return arg0;
}

void func_00178250(void *arg0) {
    *(s32 *)((char *)arg0 + 0x680) = 1;
    *(s32 *)((char *)arg0 + 0x684) = 1;
}

typedef struct GlobT178 { u8 pad[0x3C2]; u16 f3C2; } GlobT178;
extern GlobT178 *D_008DCB20;
typedef struct Arg178440 { u8 p0[2]; s16 f2; } Arg178440;
void func_00178440(Arg178440 *arg0, s32 arg1) {
    if (arg0->f2 == -1) {
        if (arg1 != 0) {
            D_008DCB20->f3C2 &= ~0x10;
        } else {
            D_008DCB20->f3C2 |= 0x10;
        }
    } else {
        D_008DCB20->f3C2 |= 0x10;
    }
}

extern s32 strchr(s32, s8);
s32 D_00224740;
typedef struct Iter178 { u32 f0; s8 *f4; s8 *f8; } Iter178;
void func_001781E0(Iter178 *p) {
    s32 cond;
loop:
    cond = p->f8 < p->f4;
    if (cond) {
        cond = strchr(D_00224740, *p->f8) != 0;
    }
    if (cond) {
        p->f8++;
        goto loop;
    }
}
