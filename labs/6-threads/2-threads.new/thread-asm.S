/*
 * implement these.  ordered in difficulty.
 */
#include "rpi-asm.h"

@ return the current stack pointer.
MK_FN(rpi_get_sp)
	bx lr

@empty stub routines.  use these, or make your own.
MK_FN(rpi_cswitch)

    @ put your cswitch here.

    @ save code
    stmia r0, {r4-r12, r14}
    str r13, [r0, #0x28]
    
    @ restore code
    ldmia r1, {r4-r12, r14}
    ldr r13, [r1, #0x28]

    bx lr

@ [Make sure you can answer: why do we need to do this?]
@
@ use this to setup each thread for the first time.
@ setup the stack so that when cswitch runs it will:
@	- load address of <rpi_init_trampoline> into LR
@	- <code> into r1, 
@	- <arg> into r0
@ 
MK_FN(rpi_init_trampoline)
    @ ldr r14, [r1, #0x48]
    ldr r0, [r1, #0x4C]
    ldr r14, =rpi_exit
    ldr r15, [r1, #0x48]

    @ bx lr
