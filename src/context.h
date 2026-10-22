/* ===== 2026-10-22: COMPILER UPGRADED =====
 * The compiler now emits sq/lq (128-bit) callee-saves like the retail build.
 * sq-callee-save (saving $s0-$s7/$fp with sq) is NO LONGER a floor -- it is MATCHABLE.
 * Older notes calling such functions unmatchable are OBSOLETE; ignore them.
 * A nonzero score is NORMAL decomp distance -- improve the C, do not release as a floor.
 * ===================================================== */
/* context.h for SLUS-20407: shared by every function. Agents grow it with edit_context:
 * add a prototype or struct the moment you work one out, so the next function benefits. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
  typedef unsigned short u16;
  typedef int s32;
  typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile s32 vs32;
typedef int s32_bool;

// Function prototypes for small functions
void func_00147940_call(u8 *dst, const u8 *src, u32 len);
void func_00147950(void *dst, void *src);
f32 func_001478F0(void);
void *func_00147A20(void *arg0, s64 arg1);
s32 func_00147490(u32 arg);

#define NULL ((void *)0)
#define TRUE 1
#define FALSE 0
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

/* This game keeps small globals (8 bytes or less) in $gp-relative small data. Declare those here WITHOUT
 * `extern` (for example `s32 D_8001B278;`): then the compiler reads them through $gp, like the game does.
 * With `extern`, it builds the full address instead and the function can't match. */

/* Small data declarations (placed in $gp-relative area) */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/* Function prototypes */
void operator_delete(void *arg0);
void func_001033F8(u32, u32);
void func_0010EEE0(u32);
void func_00105C50(u32, u32, u32);
void func_00105C98(u32, u32, f32);
void func_0015FAC0(u32);
void func_0013A8B0(void);

    extern u8 D_00223750[];
    extern u8 D_00223830[];
    extern s32 D_0020074C[];
    extern s32 D_00200748[];

s32 float_to_fixed_saturated(f32);
void func_0011DC50(f32);

/* func_0011DC90: prototype for called function */
void func_0011DC90(u32);

void *func_001555F0(void *arg0, s64 arg1);
void func_001555B0(u32 arg, void *a1);
u32 func_001555A0(u32 arg);
void func_00155590(void);

void syscall_0x5b(void);
void syscall_0x5c(void);
void syscall_0x59(void);

extern u8 D_00204F88[];
extern float D_00204C00[];

#define STACK_ALLOC_16BYTE 16
#define STACK_ALLOC_32BYTE 32

/* 128-bit quadword type. A by-value u128 parameter lands in a single 128-bit GPR and a
 * store of it becomes `sq` (shows as `ext $rX,$rY,0,1` in the diff). Matched func_00105058
 * as: void f(void*a0, u128 v){ u128*p=*(u128**)a0; *p=v; ... } */
typedef unsigned int u128 __attribute__((mode(TI)));
#define STACK_ALLOC_8BYTE 8

void func_0010F8E0(void);
void func_0010F970(u32, void *, s32);
s32 func_0010F980(void *dst, void *src, u32 len);
void func_0010F090(s32 code, void *buf);
void func_0010FCE8(u32 arg);

#define SAVE_RA() __asm__ __volatile__ ("sd $ra, 0x0($sp)")
#define RESTORE_RA() __asm__ __volatile__ ("ld $ra, 0x0($sp)")
#define JUMP(target) __asm__ __volatile__ ("jal " #target)
#define SYNC() __asm__ __volatile__ ("sync")

extern u8 D_00223F20[];

typedef unsigned long long u64;
void *func_001E3D00(void *arg0, s64 arg1);

extern u8 D_001FD298[];

void func_00104E40(volatile u8 *dst, const volatile u8 *src, u32 len);
void *func_00104E60(void *arg0);
void func_00104DE0(u32 *arg0);


/* Matching trick: for serial scalar-float ops, use volatile pointers to force serial loads/neg/stores. See func_0015ABC0. */
typedef volatile f32 *vf32_p;

/* Matching trick: for independent constant stores with reused reg, use explicit local to force single-reg reuse. See func_0015AC00. */


/* Compiler quirk: ee-gcc reserves extra 0x10 stack for some functions. Use STACK_ALLOC_16BYTE where needed. */

typedef unsigned int u32;
typedef unsigned short u16;
typedef signed int s32;

typedef unsigned int uintptr_t;

extern u8 D_002240D0[];

typedef unsigned int u32;

extern u8 D_00223750[];

extern u8 D_002238B0[];
extern u8 D_002237F0[]; /* C++ vtable (base), pair of D_00223750; see func_0013D8E0 dtor */
extern u8 D_002238F0[];
extern u8 D_00223870[];

volatile s32 D_001FD308;



typedef struct { s64 high; s64 low; } s128;

int func_00124080();

void syscall_0x04(void);
void syscall_0x05(void);
void syscall_0x06(void);

extern s32 D_002238C0;
extern s32 D_00223880;

extern u8 D_8DCB00[];
extern u8 D_008DCB00[]; /* same absolute addr 0x008DCB00 as pointer arg; 8-digit form -> lui %hi;addiu %lo (signed) */
extern u8 ref_handle_vtable[]; /* 0x00223BB0; stored into RefHandle.vtbl (s32 val@0; void* vtbl@4) */

extern void* D_8DCB20;

void syscall_0x04(void);
void syscall_0x05(void);
void syscall_0x06(void);

extern u8 D_00223C30[];
extern u8 D_002239C8[];
extern u8 D_002239B8[];

typedef unsigned int u32;

typedef unsigned char u8;

void syscall_0x05(void) ;


typedef unsigned int uint32_t;



typedef unsigned long long u64;



#define M2C_FIELD(ptr, type, offset) (*((type *)((u8 *)(ptr) + (offset))))

extern u32 D_22A598;
extern u32 D_22A5C0;
extern u32 D_22A640;
extern u32 D_22BB80;

extern u32 D_001FECE4;

/* Bitstream reader (MSB-first) used by the 0x10BBxx/0x10Cxxx TU. Matched bitreader_peek_signed.
 * 0x0: u64 acc; 0x8: buffer ptr (dup of cur at init); 0xC: u8* cur; 0x10: u32 bitcount;
 * 0x18: s64 total_bits; 0x20: u8* start; 0x24: u8* end (=start+size); 0x28: u32 size. */
typedef struct BitReader {
    u64 acc;      /* 0x00 */
    u32 buf;      /* 0x08 */
    u8  *cur;     /* 0x0C */
    u32 bitcount; /* 0x10 */
    u32 pad14;    /* 0x14 */
    s64 total;    /* 0x18 */
    u8  *start;   /* 0x20 */
    u8  *end;     /* 0x24 */
    u32 size;     /* 0x28 */
} BitReader;
s32 bitreader_peek_signed(void *s, s32 n);
void bitreader_skip_refill(void *s, s32 n);
void bitreader_init(void *s, void *buf, s32 start, s32 size);
/* func_0010BC90(obj, nbits)->u32: read & consume nbits (MSB-first); func_0010BCE0(obj): align/skip marker.
 * These two + bitreader_peek_signed drive the 0x10Bxx/0x10Cxxx MPEG system/pack-header parsers.
 * func_0010C448 parses an MPEG pack header: reads 0x22,3,align,0xF,align,0xF,align,9(->out0x0),0x1E(disc),3(->count);
 *   out0x8=(b3>>2)&1; out0x4=(b3<<0x1E)|(b15a<<0xF)|b15b; skip count bytes; if peek32()==0x1BB out0xC=1,func_0010C598(obj,out) else out0xC=0; return 1. */
u32 func_0010BC90(void *obj, s32 nbits);
void func_0010BCE0(void *obj);
void func_0010C598(void *obj, void *out);

extern u32 D_22A598;

extern u32 D_001FECFC;
u32 func_00119470(u32);

s32 *func_00111B30(s32 arg0, void *arg1);

/* func_00111B80 (claude-10): handle/request setup, sibling of func_001119D8 family (same
 * 0x10-sd-slot-spacing FLOOR, best 1180/73.1%, body 100% correct). Allocates dst=func_00111850(arg1);
 * copies arg0->{0x14,0x1C} to dst, sets dst->0x20=0x80000009; node=func_00111B30(arg0->0x20,arg1);
 * if(node){dst->0x24=node; dst->0x28=node[2]; dst->0x2C=node[5];} else zero those 3;
 * tail: return func_001113A8(0x80000008, dst, 0x40, 0,0,0). Floor = frame 0x40 (regs saved at 0x10
 * spacing; harness packs 0x8->0x20) + ROM bnel vs harness bnez + tail-arg scheduling cascade. */
void *func_00111850(s32 arg0);
s32 func_001113A8(s32 a, void *b, s32 c, s32 d, s32 e, s32 f);

extern u8 D_00224010[];

extern void* D_2286C0;
extern void* D_001FDE28;
extern void* D_2288E8;
extern void* D_001FD5CC;
extern void* D_001FDE30;
extern void* D_001FE1B8;
extern void* D_001FDE48;

extern u32 D_22A598;

u32 func_0011A5B8(s32, s32, s32, s32, void *);

extern void* D_00213E28;

void func_00116080(s32, s32, s32, void *);

extern u8 D_00228D68[];

extern u8 D_001FDE44[];

extern void* D_8EDF28;

typedef struct { u32 f0, f4, f8, fc, f10, f14; } FooB4; /* 24 bytes */
FooB4 *D_00224A58;
s32 D_00224A5C;
extern u32 D_00224A48[];

#define D_0020F660 ((char*)0x8EEF660)
#define D_0020F640 ((char*)0x8EEF640)

extern int *const g_C27160;

extern int *D_C27160;
extern u8 *D_224A48;

extern int *D_C27160;
extern u8 *D_224A48;

extern void *D_800E8358;

/* func_00118968: fills request struct D_22A280 then calls func_00111E00 (9-arg, 8-GPR+1stack).
 * D_22A280 is a registered BSS struct at 0x22A280. func_00111E00 args in $4-$11 + stack[0]. */
typedef struct {
    s32 field0;   /* 0x0 (set to 8) */
    s32 field4;   /* 0x4 */
    s32 field8;   /* 0x8 */
    u8  data[8];  /* 0xC (6 bytes copied in) */
    s32 field14;  /* 0x14 result */
} Struct_22A280;
extern Struct_22A280 D_22A280;
extern u8 D_22A040[];
s32 func_00111E00(void *, s32, s32, void *, s32, void *, s32, s32, s32);
void func_00118388(s32, s32, s32);
s32 func_00118968(s32 arg0, s32 arg1, u8 *arg2);

extern u8 _ctype_[0x100];

void func_001A5AC0(void);

typedef unsigned char u8;

/* func_00116B38: sibling of func_00118968 (fills a request struct, calls func_00111E00).
 * Checks func_00115EB8()<0 -> return 0xFFFF0000; writes D_00228B40.{field0=arg0, field4=arg2type};
 * stores *arg1 at field8 sized by arg2 (0=byte,1=half,2=word, else return 0xFFFEFFFE);
 * func_00111E00(&D_00228D40,2,0,&D_00228B40,0x20,&D_00228B40,0x10,0,0); return >=0?0:0xFFFEFFFF. */
typedef struct { s32 field0; s32 field4; s32 field8; } Struct_228B40;
extern Struct_228B40 D_00228B40;
extern u8 D_00228D40[];
s32 func_00115EB8();
s32 func_00116B38(s32 arg0, u8 *arg1, s32 arg2);

/* func_001168F8 (claude-11): sibling of func_00116B38 — two-name request builder, sd-spacing FLOOR
 * (best 2118/67.4%, body correct). D_00228B48 = &D_00228B40 + 8 (char buffer; struct base = D_00228B40,
 * two s32 result fields at base+0/+4, name1[0xFC]@+8, name2[0xFC]@+0x104, total 0x200).
 *   if(func_00115EB8()<0) return 0xFFFF0000; if(func_00115FB8()!=0) return 0xFFFEFFFC;
 *   strncpy(name1,arg0,0xFC); name1[0xFB]=0; strncpy(name2,arg1,0xFC); name2[0xFB]=0;
 *   if(func_00111E00(&D_00228D40, arg3, 0, base, 0x200, base, 0x10, 0, 0) < 0) return 0xFFFEFFFF;
 *   if(base[0]==0) return 0xFFFEFFFD; arg2[0]=base[0]; arg2[1]=base[1]; return 0;
 * Trick: name1 = (u8*)&D_00228B40 + 8 folds the +8 into %lo -> reloc resolves D_00228B48 (addiu -0x74b8),
 * matching ROM's name1-primary anchor with base = name1-8. Floor = frame 0x70 (5 s-regs+ra at 0x10 sd
 * spacing) vs harness 0x40 (0x8 packing) + beql/bnel branch-likely + name1/base regalloc swap cascade. */
s32 func_001168F8(void *arg0, void *arg1, s32 *arg2, s32 arg3);

extern u8 D_0021D1E9[256];

typedef void (*func_ptr_t)();

extern u16 D_00222020[441];
extern s32 D_002118D0[441];

/* Absolute RAM address access pattern: for globals at 0x8Dxxxxx/0x8Exxxxx or
 * other absolute addresses not in symbol table, use *(type*)addr pattern.
 * Example: (s32 *)0x1000E000 for absolute loads like lui $at,%hi; lw ...%lo($at)
 */

void func_00166B20(void *, s32);
s32 func_00177B40(void *, void *, s32);
void func_00182910(void *, void *);
void func_0011EC80(void *, float);
void func_0011DA70(void *, float);

void func_001065C8(void *, void *, float);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    // ... other fields at 0xC, 0x10, etc.
} struct_0x8CCB00;

/* func_00111C30 family (handle/node setup; 12 callers). Decoded & verified by claude-9.
 * func_00111788(void*) allocates a node (returns ptr or NULL); func_00111830(void*) frees it.
 * func_00111368(0x80000009, node, 0x40, 0,0,0) registers it (returns 0 on success here).
 * func_0010EC60(void*) / func_0010EC70(s32) / func_0010ECA0(s32) are the critical-section/handler trio. */
void *func_00111788(void *tag);
void func_00111830(void *node);
s32 func_00111368(s32 a, void *node, s32 c, s32 d, s32 e, s32 f);
s32 func_0010EC60(void *req);
/* func_001119D8 (claude-8): handler/callback registration. Allocates a node via
 * func_00111788(D_00227580); stores node into arg0->0x0 and node->0x18 into arg0->0x4;
 * fills node->{0x20=arg1,0x24=arg2,0x28=arg3,0x14=node(self),0x1C=arg0}. If !(arg4&1):
 * func_0010EC60(&EcReq{field4=1,field8=0})->arg0->0x8; <0 -> free+(-3); then
 * func_00111368(0x8000000C,node,0x40,0,0,0): ==0 -> free+EC70(arg0->8)+(-2),
 * !=0 -> ECA0(arg0->8)+EC70(arg0->8)+0. Else(arg4&1): arg0->0x8=-1; func_00111368(same):
 * !=0 -> 0, ==0 -> free+(-2). Body 100% matched; floored at 1217/85.8% by 16-byte sd-slot
 * spacing (frame 0x90, saves $s0-$s5+$ra at 0x10 spacing; harness packs 0x8 -> frame 0x50). */
s32 func_001119D8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_0010EC70(s32 id);
void func_0010ECA0(s32 id);
extern u8 D_00227580[];

/* func_0011AAC0: command-packet builder (fully decoded by claude-5, 2026-10-21).
 * Checks func_0010ECB0(D_0020074C[0])<0 -> -0xC8; D_0022A5C0.field24==0 -> cleanup+-0x64;
 * arg2 NULL/empty -> cleanup+-0xD2. Else strncpy(D_0022A684,arg2,0x3FF) and fills a
 * header at D_0022A684-0x14 {cmd@0=arg0, a@4=arg1, b@8=arg3, term@0x413=0}, then
 * func_00111E00(&D_0022A5C0,2,1,hdr,0x414,&D_0022BB80,4,0,0); ok -> D_00200748[0]=2. */
s32 func_0010ECB0(s32);
void func_0010EC80(s32);
void *func_001D4230(void *dst, void *src, u32 n); /* strncpy-like (returns dst) */

/* func_0011B888 family (siblings 0x0011AAC0/BE70/BC40/B490 are 90-95% similar: fill a request
 * buffer then call func_00111E00). func_0010ECB0(handle)->s32 (<0 = error); func_0010EC80(handle)
 * releases/cleans up. D_0020074C[0] is a global handle. Request struct at D_0022A684-0x14:
 *   {s32 field0@0x0; s32 field4@0x4; s32 field8@0x8; char name[0x400]@0x14}; name null-term at [0x3FF].
 * D_0022A5C0 is a manager struct with an s32 at 0x24 (nonzero = ready). */
s32 func_0010ECB0(s32);
void func_0010EC80(s32);

/* func_0010DA68 (parser/config init over an object at arg0; tail-calls func_0010DB90(obj->0x858)).
 * func_00109B98(obj, nbits)->u32 is a bitfield/register READER (reads 0x20,0x1E,1 bits).
 * func_001096D0(obj) / func_001096A0(obj, u32 base) / func_0010DF18(obj, u32 base, void* table)
 * configure two regions at bases 0x50000000 and 0x58000000; func_0010D900(obj, void* table). */
u32 func_00109B98(void *obj, s32 nbits);
void func_001096D0(void *obj);
s32 func_001096A0(void *obj, u32 base);
void func_00109FA8(void *obj);
void func_0010D900(void *obj, void *table);
s32 func_0010DB90(s32);
void func_0010DF18(void *obj, u32 base, void *table);
void func_001084C8(s32);
/* func_0010DFF0 (body fully solved by claude-2, floor-blocked at 1769): sequence header parser.
 * obj->0x848=1; func_001084C8(0); v=func_00109B98(obj,0x1C) (28-bit read);
 *   obj->0x140=(v>>0x11)&3, if !=1 func_0010D900(obj,&D_00213EF0);
 *   obj->0x13C=(v>>0x13)&1; w=func_00109B98(obj,0x10)>>8; code=v>>0x14;
 *   if code not in {0x48,0x58,0x44} func_0010D900(obj,&D_00213F18);
 *   obj->0x124=(((v>>0xF)&3)<<0xC)|(old&0xFFF); obj->0x128=(((v>>0xD)&3)<<0xC)|(old&0xFFF);
 *   obj->0x134+=((v>>1)&0xFFF)<<0x12; obj->0x138+=w<<0xA. */

/* func_00133420: resource relocate/fixup (claude-8). func_001D16A0(dst,val,len)=memset.
 * Takes dst struct + loaded source header a1; relocates a1[0]-counted pointer array at a1+4
 * (each += a1), then fixes sub-objects: a0->f0=a1[1] (sprite: w@0x14,h@0x16 s16, ptrs@0x18/0x1C
 * +=self, w*h tile-ptr array @0x20 nonzero entries +=a0->f0); a0->f40=a1[2] (f4/f8/fC +=self);
 * a0->f48=a1[3]; a0->f44=a1[4]; a0->f4C=a1. BODY FULLY MATCHES; floored only by prologue config. */
void func_001D16A0(void *dst, s32 val, s32 len); /* memset(dst, val, len) */

/* func_0019C590 (claude-1, sq-callee-save FLOOR, body fully decoded). Copies a 16-aligned
 * 300-byte table D_00207CF0 (s32[5][15] of sprite/frame IDs) to a stack local, then:
 *   p=*(void**)(arg1+0x28); if(!p)return; idx=*(u16*)(p+0x26); if(idx>=15)return;
 *   frame=table[arg0][idx]; v0=v1=0x1EFF; func_0019CC50(&v0,&v1,mode);
 *   if(v0<11 && v1<11) return;
 *   mode==0 ? func_001B3F50(D_00224A28,frame,0x1000,v0,v1)
 *           : func_001B3F50(D_00224A28,frame,(func_001D2930()&0xFF)+0xF81,v0,v1);
 * func_0019CC50 is 3-arg (m2c over-counted to 6 from leftover copy-loop regs). */
typedef struct { s32 v[5][15]; } __attribute__((aligned(16))) Tbl_19C590;
extern const Tbl_19C590 D_00207CF0;
void func_0019CC50(s32 *a, s32 *b, s32 mode);
void func_001B3F50(void *ctx, s32 frame, s32 color, s32 d, s32 e);
s32 func_001D2930(void);
extern void *D_00224A28; /* gp-rel small-data ptr passed as ctx to func_001B3F50 */

/* func_001AC2F0 (claude-6): C++ ctor-style setup. sq-callee-save+loop-reversal FLOOR (body faithful).
 * func_00105CF0(void* dst0x30, float* tbl) inits a 0x30-byte struct from float table D_00204C00.
 * func_001708F0(void* handle, s32* out) extracts a word via a ref-handle at arg0.
 * func_0018EA20 (already declared elsewhere) submits a draw/job: (*(s32*)0x8DCB4C, val, params0x30).
 * ref-handle stack idiom: 8-byte {s32 val; void* vtbl} objects, vtbl always = ref_handle_vtable
 *   (0x00223BB0); func_00166B20 fills .val, then .vtbl set, then copied field-wise into another.
 * tail is the shared-ptr copy idiom: dst->ptr=src->ptr; if(dst->ptr){ dst->ref=src->ref; (*dst->ref)++; } */
void func_00105CF0(void *dst, void *floatTable);
void func_001708F0(void *handle, s32 *out);

/* Software bigfloat (custom soft-float) — used across the 0x1DFxxx TU and the 12 callers of
 * bigfloat_div (0x1DFBD0). Packed form is an s64; bigfloat_unpack(s64* src, dst) fills this
 * 0x20-byte struct; bigfloat_pack(src) returns the packed s64. bigfloat_nan (0x21E058) = NaN const.
 * class(cls@0x0): 0=NaN, 1=(special), 2=zero, 3=normal, 4=infinity. cls<2 -> propagate operand as-is.
 * (bigfloat_unpack/pack already declared elsewhere; do not redeclare.) */
typedef struct {
    u32 cls;    /* 0x0  fpclass */
    s32 sign;   /* 0x4  */
    s32 exp;    /* 0x8  */
    u32 bf_pad; /* 0xC  */
    u64 mant;   /* 0x10 mantissa */
    u64 bf_pad2;/* 0x18 */
} BigFloat;

/* func_00183750 (claude-7, body fully decoded; sq-callee-save+vtbl-CSE FLOOR, best 6579).
 * 5 params (a1,a2 unused): a0=out ref-handle pair dst, a3=src ref-handle pair, a4=obj-with-count.
 * Builds two 8-byte ref-handles {s32 val; void* vtbl=ref_handle_vtable} on the stack via
 * func_00166B20(&h,a4)/func_00177B40(&dst,&src,0)/func_001708F0(&h,&outIdx); looks up
 * obj=func_001349E0(D_008DCB00,outIdx); for i in [0, a4->0x8 - 1): sets bit (1<<idx) into the
 * s64 at obj+0xCA8 and mirrors it to obj+0xCA0. Then copies a3->0 to a0->0 and, if nonzero,
 * a3->4 to a0->4 and increments (*(s32*)a0->4). ref_handle_vtable=0x00223BB0; D_008DCB00 abs. */
s32 func_00183750(void *a0, s32 a1, s32 a2, void *a3, void *a4);
extern u8 ref_handle_vtable[];
extern u8 D_008DCB00[];
void *func_001349E0(void *tbl, s32 idx);

/* func_00144030 (claude-7): C++ deleting destructor, triple-floor family (sibling of
 * func_0013C1B0/0013D8E0/0013B0A0). Best 1457. this=$a0, flag=$a1 (s64).
 *   if(this){ this->0xC=&D_00223970; (inner if(this)) this->0xC=&D_00223750;
 *             if(((flag<<48)>>48)>0) operator_delete(this); } return this;
 * D_00223970/D_00223750 are vtables. Floor: sq-save($16, frame 0x20 vs harness sd 0x10) +
 * folded redundant inner beqz + dsra32 dropped by the `>0` fold. Single-volatile store1 recovers DSE. */
void *func_00144030(void *this, s64 flag);
extern u8 D_00223970[];
extern u8 D_00223960[]; /* C++ vtable A for deleting-dtor func_001480C0 (store A to this->0xC, B=D_00223750) */
extern u8 D_00223950[]; /* 0x148-family deleting-dtor vtableA, pairs with D_00223750; see func_00148060 */
extern u8 D_002240E0[]; /* 0x1F8-family deleting-dtor vtableA, pairs with D_00223750; see func_001F8330. 1457 floor. */

/* func_0011FFA0 (claude-10): exact TWIN of func_00144030, vtables at offset 4 (not 0xC).
 * Deleting-destructor triple-floor family; best 1457 (sq-save + folded inner beqz + dropped dsra32).
 *   void* f(void* this, s64 flag){ if(this){ *(volatile u32*)(this+4)=&D_002235F0;
 *     if(this) *(u32*)(this+4)=&D_00223650; if(((flag<<48)>>48)>0) operator_delete(this); } return this; }
 * D_002235F0/D_00223650 are C++ vtables (base/derived). volatile store1 + single-expr shift = 1457 floor. */
void *func_0011FFA0(void *this, s64 flag);
extern u8 D_002235F0[]; /* C++ vtable (base) for func_0011FFA0 deleting-dtor */
extern u8 D_00223650[]; /* C++ vtable (derived); shared by func_0011FFA0 AND func_00120060 deleting-dtors */

/* func_00120060 (claude-10): THIRD twin of func_00144030/func_0011FFA0. Same deleting-dtor
 * triple-floor family, vtables at offset 4: base D_00223630, derived D_00223650 (same derived).
 * Reproduced exact 1457 floor with claude-10's recipe:
 *   void* f(void* this, s64 flag){ if(this){ *(volatile u32*)(this+4)=&D_00223630;
 *     if(this) *(u32*)(this+4)=&D_00223650; if(((flag<<48)>>48)>0) operator_delete(this);} return this; }
 * Floor elements (all confirmed non-C-steerable): sq-save of $s0 (ext $s0,$sp,0,1; frame 0x20 vs
 * harness sd/0x10) + harness folds the redundant inner beqz + harness drops dsra32 (proves
 * (flag<<48)>0 == ((flag<<48)>>48)>0). volatile store1 recovers both vtable writes (defeats DSE). */
void *func_00120060(void *this, s64 flag);
extern u8 D_00223630[]; /* C++ vtable (base) for func_00120060 deleting-dtor */

/* func_001B3530 (claude-10, decoded; sq-callee-save FLOOR, best 3614/62.7%). Sets up a
 * turn/orient animation state. arg0 big object; sub = arg0+0xCE0 (anim-state struct).
 * func_0012AE90(obj, cmd, p, q): command dispatch (cmd 0x11 bail, 5 = begin w/ matrix+angle,
 *   0x15 = step). func_00105C68(out, srcA, srcB): builds a transform from two 0x388-ptr+0x30 bufs.
 *   func_001D2930()->s32: input/dir bits (uses &0x30, &0x10 for sign). 0x3EC90FDB = pi/8 angle.
 * Logic: if !(flags0x3C8 & 0x300) -> cmd 0x11,dir 1; else if &0x100 -> cmd 0x11,dir 2; else
 *   n=sub->0x24; if n==0 { build xform, cmd5(obj,&xform,&angle), zero sub, pick sign +-1.0f into
 *   sub->0x8 (both bits set/clear) or sub->0x0 (one bit), n=0x14; } cmd 0x15; sub->0x24=n-1.
 * SubCE0: 0x0 f32,0x4 s32,0x8 f32,0xC s32,0x18 s32,0x1C void*(self/dir),0x20 s32,0x24 s32 counter.
 * Float consts stored as INT bit-patterns (lui+sw/lui+ori+sw), NOT float literals (avoids mtc1). */
void func_0012AE90(void *obj, s32 cmd, void *p, void *q);
void func_00105C68(void *out, void *srcA, void *srcB);

/* func_001B59F0 (claude-10): FOURTH twin of func_00144030/0011FFA0/00120060 deleting-dtor
 * family. Vtables at offset 0x0: base D_00223E80, derived D_00223E60. Reproduced exact 1457
 * floor with claude-10's recipe:
 *   void* f(void* this, s64 flag){ if(this){ *(volatile u32*)this=(u32)&D_00223E80;
 *     if(this) *(u32*)this=(u32)&D_00223E60; if(((flag<<48)>>48)>0) operator_delete(this);} return this; }
 * Floor (all non-C-steerable): sq-save $s0 (ext $s0,$sp,0,1; frame 0x20 vs harness sd/0x10) +
 * harness folds inner beqz + harness drops dsra32. volatile store1 recovers vtable write. */
void *func_001B59F0(void *this, s64 flag);
extern u8 D_00223E60[]; /* C++ vtable (derived) for func_001B59F0 deleting-dtor */
extern u8 D_00223E80[]; /* C++ vtable (base) for func_001B59F0 deleting-dtor */

/* dtoa / David Gay floating-point conversion module (0x1D2xxx family), d2b verified by claude-11.
 * d2b (0x1D2580): decompose a double's bit-pattern into a Bigint; returns b, writes *e,*bits.
 *   Bigint layout: s32 wds @ 0x10; u32 x[] @ 0x14 (x[0]@0x14, x[1]@0x18).
 *   Balloc(ti,k)->Bigint*; lo0bits(u32* y)->count and shifts *y right by count; hi0bits(u32)->leading-zero count.
 *   ti = thread-info ptr passed through as d2b's first arg. Const: 0x433=1075, 0x432=1074, 0x35=P=53,
 *   0x100000=implicit bit, 0xFFFFF=Frac_mask. d2b body logically exact, floored ~935/90.3% by regalloc+
 *   scheduling. Best recipe: union {s64 ll; u32 L[2];}, L[1]&=0x7FFFFFFF to clear sign in-register
 *   (NOT an explicit 64-bit mask — that cascades regalloc). (Helper prototypes live in matched sources.) */

/* func_00191560 (claude-11): LARGER twin of func_00183750 ref-handle idiom; same per-TU
 * sq-callee-save + aggressive-CSE DOUBLE FLOOR (best 7316/27.5%, body 100% correct).
 * sig: s32 func_00191560(void* arg0, s32 a1(unused), s32 a2(unused), void* arg3, s32 arg4); a4=$t0.
 * Builds src ref-handle {val@sp50, vtbl@sp54} from func_00166B20(&tmp{sp58},arg4); copies 4 derived
 * handles h1..h4, extracting: func_001708F0(&h1,&i1); func_001708F0(&h2,&i2); func_00182910(&h3,&f3);
 * func_00182910(&h4,&f4) (func_00177B40(&hN,&src,0) before each; hN.vtbl=ref_handle_vtable after).
 * Then r1=func_001349E0(D_008DCB00,i1); r2=func_001349E0(D_008DCB00,i2);
 * func_0011EB20(&D_0022BEE0, r1, r2, f3, f4); func_0011DA70(*(void**)0x8DCB34, &D_0022BEE0);
 * then shared-ptr copy tail: *arg0=*arg3; if(*arg0){ arg0[4]=arg3[4]; (*(s32*)arg0->4)++; } src.vtbl=vtbl.
 * FLOOR: harness ee-gcc 2.96 -O2 CSE-hoists D_008DCB00 addr + the ref_handle_vtable value into
 * callee-saved regs (survive calls); ROM's weaker optimizer recomputes lui%hi/addiu%lo FRESH after
 * every call (all 4 saved regs spent on arg0/arg3/r1/&sp54) AND uses `sq` for the 4 GPR saves
 * (frame 0x90) vs harness `sd` (frame 0x60) -> every local offset shifts. Neither C-steerable.
 * NOTE func_0011DA70 is really (void*, void*) here (2nd arg = &D_0022BEE0, a pointer), NOT the
 * (void*,float) context prototype — call via ((void(*)(void*,void*))func_0011DA70)(..) to avoid the
 * conflict. D_0022BEE0 links only as the 8-digit name (D_22BEE0 is unknown to the linker). */
s32 func_00191560(void *arg0, s32 a1, s32 a2, void *arg3, s32 arg4);
f32 func_0011EB20(void *obj, void *a, void *b, f32 x, f32 y);
extern u8 D_0022BEE0[];

/* func_00111C30: handle/subsystem open. (ptr, id=0x80000001, 0) -> s32 (<0 = fail). Used by
 * the 0x00112xxx/0x00113xxx-0x00115xxx init-function family (18+ callers of func_00112B88). */
s32 func_00111C30(void *obj, s32 id, s32 flags);

/* dlmalloc (Doug Lea malloc 2.6.x variant) — decoded by claude-10 from malloc_free = fREe (free()).
 * malloc_chunk: 0x0 prev_size, 0x4 size (bit0=PREV_INUSE), 0x8 fd, 0xC bk.
 * Arena/bins base = malloc_av (av0): *(av0+4)=binblocks(u32 bitmap), *(av0+8)=top(mchunkptr).
 *   bin_at(i) = (mchunkptr)(av0 + i*8); its fd/bk live at +8/+0xC. last_remainder = bin_at(1) = av0+8.
 *   smallbin_index(sz)=sz>>3; large bin_index per Lea macro (56+sz>>6 / 91+sz>>9 / 110+sz>>12 /
 *   119+sz>>15 / 124+sz>>18 / 126); mark_binblock sets 1<<(idx>>2) (small) or 1<<(idx/4) (large,signed).
 * NOTE the ROM references the SAME bins base three ways: malloc_av (small/top/binblocks),
 *   D_00210F58-8 (large-bin base), D_00210F60-8 (last_remainder) — distinct %hi/%lo symbol forms.
 * D_00211358 (u64 trim_threshold) and D_00211360 (s32 top_pad) are FULL-ADDRESS (>G8 or array) —
 *   declare `extern u64 D_00211358[]` / `extern s32 D_00211360[]` (plain extern scalar -> gp_rel, wrong).
 * Helpers (declare LOCALLY in your source to avoid conflicts): func_001D1760()=lock-enter (void);
 *   func_001D17E0(void*av)=lock-leave (tail-called); func_001CFDE8(void*av,s32 top_pad)=trim. */
struct malloc_chunk { s32 prev_size; s32 size; struct malloc_chunk *fd; struct malloc_chunk *bk; };
