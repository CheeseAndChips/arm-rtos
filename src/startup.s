.section .vectors,"a"
_vectors:
    .word _estack               /* end of stack  */
    .word _reset                /* reset handler */
    .word _default_err_handler1 /* non-maskable interrupt */
    .word _default_err_handler2 /* hardfault */
    .word _default_err_handler3 /* memory management */
    .word _default_err_handler4 /* pre-fetch/mem access fault */
    .word _default_err_handler5 /* undefined instruction */
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
    LDR r0, =_sidata
    LDR r1, =_sdata
    LDR r2, =_edata

    CMP r1, r2
    BEQ end_loop 
copy_loop:
    LDR r3, [r0]
    STR r3, [r1]
    ADD r0, #4
    ADD r1, #4
    CMP r1, r2
    BNE copy_loop

end_loop:
    BL c_entry
    B .

_default_err_handler:
    B .
_default_err_handler1:
    B _default_err_handler
_default_err_handler2:
    B _default_err_handler
_default_err_handler3:
    B _default_err_handler
_default_err_handler4:
    B _default_err_handler
_default_err_handler5:
    B _default_err_handler
_empty_handler:
    /* return instantly */
    BX LR
