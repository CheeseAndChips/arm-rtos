ENTRY(reset_handler)
SECTIONS
{
    . = 0x10000;
    .startup . : { CMakeFiles/arm-rtos.dir/src/startup.s.obj(.text) }
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss COMMON) }
    . = ALIGN(8);
    . = . + 0x1000;
    stack_top = .;
    . = . + 0x1000;
    irq_stack_top = .;
}
