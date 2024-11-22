[bits 16]

switch_to_pm:
  cli
  lgdt [gdt_descriptor]
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  jmp CODE_SEG:pm_init

[bits 32]

pm_init:
  mov ax, DATA_SEG
  mov ds, ax
  mov es, ax
  mov ss, ax

  jmp KERNEL_START_ADDR

%include "gdt.inc"
