/*
 * implement these.  ordered in difficulty.
 */
#include "rpi-asm.h"

@ return the current stack pointer.
MK_FN(callee_save)
	bx lr

@ store the second parameter into the memory 
@ pointed to by the first parameter
MK_FN(store_one_asm)
    push {lr}
    str r1, [r0]
    pop {pc}


@ implement this routine
MK_FN(check_callee_save)
    @ first: destroy all registers by loading 4 into r4, 
    @ 5 into r5 etc for each saved register.
    mov r4, #4
    mov r5, #5
    mov r6, #6
    mov r7, #7
    mov r8, #8
    mov r9, #9
    mov r10, #10
    mov r11, #11
    mov r12, #12
    mov r14, #14


    @ second: save the registers: since we set them to known 
    @ values we can print them and check everything got
    @ written.
    #str r4, [r0, #0]
    #str r5, [r0, #4]
    #str r6, [r0, #8]
    #str r7, [r0, #12]
    #str r8, [r0, #16]
    #str r9, [r0, #20]
    #str r10, [r0, #24]
    #str r11, [r0, #28]
    #str r12, [r0, #32]
    #str r13, [r0, #40]
    #str r14, [r0, #36]
   
    stmia r0, {r4-r12, r14}
    str r13, [r0, #0x28]

    @ call print_and_die
    bl print_and_die


@
@ simple context switch:
@     cswitch(uint32_t *old, uint32_t *new)
@
MK_FN(cswitch)
    @ put your save code here

    @ kill all the regs to verify save/restore works.
    mov r4, #4
    mov r5, #5
    mov r6, #6
    mov r7, #7
    mov r8, #8
    mov r9, #9
    mov r10, #10
    mov r11, #11
    mov r12, #12
    mov sp, #13
    mov lr, #14
    
    @ put your restore code here.
    bx lr
