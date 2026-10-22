/* Data declarations - use array notation for absolute access, or extern globals for gp-relative */
/* For gp-relative data, we can use extern s32 and gcc should place them properly */
extern s32 D_00224898;
extern s32 D_0022489C;
extern s32 D_002248A0;
extern s32 D_002248A4;
extern s32 D_002248A8;
extern s32 D_002248AC;
extern s32 D_002248B0;
extern s32 D_002248B4;

/* Absolute address data */
extern u8 D_0072C114[];
extern u8 list_iterator_vtable[];

void func_00188420(void *addr, s32 value);

/* func_00133D00: simple load from gp-relative address */
s32 func_00133D00(void) {
    return D_00224898;
}

/* func_00133D10: match exact control flow and register usage */
/* ROM structure:
   lw $v1, -0x7858($gp)   ; val1
   beqz $v1, .Lskip
   nop
   lw $a1, -0x783c($gp)   ; val2 -> $a1
   sw $zero, -0x7850($gp) ; D_002248A0 = 0
   sw $zero, -0x7854($gp) ; D_0022489C = 0
   beqz $a1, .Lskip2      ; if val2 == 0, skip inner
   sw $zero, -0x784c($gp) ; D_002248A4 = 0 (in delay)
   lui $a0, 0x73
   addiu $a0, -0x3eec
   jal func_00188420
   sw $zero, -0x783c($gp) ; D_002248B4 = 0 (after call)
   sw $zero, -0x7848($gp) ; D_002248A8 = 0
   sw $zero, -0x7844($gp) ; D_002248AC = 0
   sw $zero, -0x7840($gp) ; D_002248B0 = 0
   sw $zero, -0x7858($gp) ; D_00224898 = 0
   .Lskip:
   .Lskip2:
   ...
*/
void func_00133D10(void) {
    s32 val1 = D_00224898;
    
    if (val1 != 0) {
        s32 val2 = D_002248B4;
        
        D_002248A0 = 0;
        D_0022489C = 0;
        
        if (val2 != 0) {
            D_002248A4 = 0;
            func_00188420((void *)D_0072C114, val2);
            D_002248B4 = 0;
        }
        
        D_002248A8 = 0;
        D_002248AC = 0;
        D_002248B0 = 0;
        D_00224898 = 0;
    }
}

/* list_begin: need to match exact register allocation
   ROM:
     lw $a1, 4($a1)       ; load from arg1+4, result in $a1 (which was arg1)
     lui $v1, %hi(list_iterator_vtable)
     addiu $v1, %lo
     daddu $v0, $a0, $zero ; $v0 = arg0 (return value)
     sw $v1, 4($v0)       ; store address at arg0+4
     jr $ra
     sw $a1, ($v0)        ; store val at arg0+0 (delay slot)
     
   The key is: $a1 is used to hold the loaded value, and $v1 holds the address.
   The stores are: first sw $v1, 4($v0), then sw $a1, ($v0) in delay slot.
*/
void *list_begin(void *arg0, void *arg1) {
    void *addr = (void *)list_iterator_vtable;
    
    /* Need to store address first, then value (value in delay slot) */
    *(void **)(arg0 + 4) = addr;
    *(s32 *)(arg0 + 0) = *(s32 *)(arg1 + 4);
    
    return arg0;
}

void func_001335E0(void) {
    /* Not implemented yet */
}