.code 32

.global isr_vectors
isr_vectors:
    LDR pc, reset_handler_addr
    LDR pc, unknown_instr_addr
    LDR pc, swi_handler_addr
    LDR pc, prefetch_abort_handler_addr
    LDR pc, data_abort_handler_addr
    LDR pc, invalid_addr_handler
    LDR pc, irq_handler_addr
    LDR pc, fiq_handler_addr

reset_handler_addr:
    .word reset_handler
unknown_instr_addr:
    .word c_irq_handler
swi_handler_addr:
    .word c_irq_handler
prefetch_abort_handler_addr:
    .word c_irq_handler
data_abort_handler_addr:
    .word c_irq_handler
invalid_addr_handler:
    .word c_irq_handler
irq_handler_addr:
    .word c_irq_handler
fiq_handler_addr:
    .word c_irq_handler

.global reset_handler
reset_handler:
    LDR r0, =isr_vectors
    MOV r1, #0x0

    LDMIA r0!, {r2-r7}
    STMIA r1!, {r2-r7}
    LDMIA r0!, {r2-r7}
    STMIA r1!, {r2-r7}

    MRS r0, cpsr
    BIC r0, r0, #0x1F
    ORR r0, r0, #0x12|0xC0
    MSR cpsr, r0
    LDR sp, =irq_stack_top

    MRS r0, cpsr
    BIC r0, r0, #0xDF
    ORR r0, r0, #0x1F
    MSR cpsr, r0

    LDR sp, =stack_top
    BL c_entry

infinite_loop:
    B .
