ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
  . = 0x1000;

  .text : {
    *(.kinit*)
    *(.text*)
    *(.rodata*)
  }

  .data : {
    *(.data*)
  }

  .bss : {
    *(.bss*)
    *(COMMON)
  }
}
