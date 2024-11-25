[bits 16]

switch_to_pm:
  lgdt [gdt_descriptor]
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  jmp CODE_SEG:flush

[bits 32]

flush:
  jmp KERNEL_START_ADDR

%include "gdt.inc"
