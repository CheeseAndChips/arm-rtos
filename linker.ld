MEMORY
{
    rom (rx): ORIGIN = 
}

SECTIONS
{
    . = 0x08000000;
    vector = .;
    . = 0x20000000;
    sram_start = .;
    . = _startup;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss COMMON) }
    . = ALIGN(8);
    . = . + 0x2000;
    sram_end = .;
}
