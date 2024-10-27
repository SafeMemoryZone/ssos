OUTPUT_FORMAT(binary)

STARTUP(build/objs/kernel/kmain_init.S.o)

SECTIONS
{
    . = 0x1000;

    .text :
    {
        *(.text)
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
        *(COMMON)
    }
}
