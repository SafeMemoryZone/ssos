ENTRY(_start)

SECTIONS
{
  . = 0x1000;

  .text : {
    KEEP(*(.kinit))
    *(.text*)
  }

  .rodata ALIGN(16) : {
    *(.rodata*)
  }

  .data ALIGN(16) : {
    *(.data)
  }

  .bss ALIGN(16) : {
    __bss_start = .;
    *(.bss)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
  }
  /* Size of .bss in DWORDS */ 
  __bss_sized = (__bss_end - __bss_start) / 4;
}
