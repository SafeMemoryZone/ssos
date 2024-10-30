OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)

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
