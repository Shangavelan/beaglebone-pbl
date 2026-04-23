MEMORY { .sram : ORIGIN = 0x40308000, LENGTH = 14K }
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)


SECTIONS
{
    .text :
    {
        /* Ensure the entry point code from start.o is at the very beginning */
        arch/start.o(.text)
        *(.text*)
    } > .sram

    . = ALIGN(4);
    .rodata : { *(.rodata*) } > .sram

    . = ALIGN(4);
    .data : { *(.data*) } > .sram

    . = ALIGN(4);
    .bss :
    {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > .sram 

    _end = .;
}
