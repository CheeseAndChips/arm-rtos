ENTRY(_reset);

MEMORY
{
    flash (rx): ORIGIN = 0x08000000, LENGTH = 512k
    sram (rwx): ORIGIN = 0x20000000, LENGTH = 128k
}
_estack = ORIGIN(sram) + LENGTH(sram);

SECTIONS
{
    .vectors(READONLY)  : { KEEP(*(.vectors)) }       > flash
    .text(READONLY)     : { . = ALIGN(4); *(.text*) }  > flash
    .rodata(READONLY)   : { *(.rodata*) }             > flash

    .preinit_array(READONLY)     :
    {
      . = ALIGN(4);
      __preinit_array_start = .;
      *(.preinit_array*)
      __preinit_array_end = .;
      . = ALIGN(4);
    } > flash

    .init_array(READONLY) :
    {
      . = ALIGN(4);
      __init_array_start = .;
      *(.init_array.*)
      *(.init_array*)
      __init_array_end = .;
      . = ALIGN(4);
    } > flash

    .fini_array(READONLY) :
    {
      . = ALIGN(4);
      __fini_array_start = .;
      *(.fini_array.*)
      *(.fini_array*)
      __fini_array_end = .;
      . = ALIGN(4);
    } > flash

    .data : {
        _sdata = .;
        *(.first_data)
        *(.data SORT(.data.*))
        _edata = .;
    } > sram AT > flash
    _sidata = LOADADDR(.data);

    .bss : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss SORT(.bss.*) COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > sram

    . = ALIGN(8);
    _end = .;
}
