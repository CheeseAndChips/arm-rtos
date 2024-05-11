.section .vectors,"a"
_vectors:
    .word _estack               /* end of stack  */
    .word _reset                /* reset handler */
    .word _default_err_handler  /* non-maskable interrupt */
    .word _default_err_handler  /* hardfault */
    .word _default_err_handler  /* memory management */
    .word _default_err_handler  /* pre-fetch/mem access fault */
    .word _default_err_handler  /* undefined instruction */
    .word 0                     /* reserved */
    .word 0                     /* reserved */
    .word 0                     /* reserved */
    .word 0                     /* reserved */
    .word _empty_handler        /* system service call via SWI */
    .word _empty_handler        /* Debug monitor */
    .word 0                     /* reserved */
    .word _empty_handler        /* pendable request for system service */
    .word _empty_handler        /* system tick timer */

.global _reset 
.section .text
.type _reset, %function
_reset:

    /* setup data */
    LDR r0, =_sidata
    LDR r1, =_sdata
    LDR r2, =_edata

    CMP r1, r2
    BEQ end_copy_loop 
copy_loop:
    LDR r3, [r0]
    STR r3, [r1]
    ADD r0, #4
    ADD r1, #4
    CMP r1, r2
    BNE copy_loop
end_copy_loop:

    /* setup bss */
    LDR r0, =_sbss
    LDR r1, =_ebss
    MOV r3, #0

    CMP r0, r1
    BEQ end_zero_loop
zero_loop:
    STR r3, [r0]
    ADD r0, #4
    CMP r0, r1
    BNE zero_loop
end_zero_loop:

    /* BL __libc_init_array */
    /* jump to main */
    BL c_entry
    B .

.global _default_err_handler
.type _default_err_handler, %function
_default_err_handler:
    B .
_empty_handler:
    /* return instantly */
    BX LR
