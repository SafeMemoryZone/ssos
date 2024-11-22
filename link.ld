ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
  . = 0x1000;

  .text : {
    KEEP(*(.kinit))
    *(.text)
  }

  .rodata ALIGN(16) : {
    *(.rodata)
  }

  .data ALIGN(16) : {
    *(.data)
    *(.bss)
    *(.COMMON)
  }
}
