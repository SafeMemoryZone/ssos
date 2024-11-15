[bits 16]

STACK_BASE_PM equ 0x9fbff 

switch_to_pm:
  cli ; disable interrupts
  lgdt [gdt_descriptor] ; load gdt
  mov eax, cr0 ; do some magic
  or eax, 1
  mov cr0, eax
  jmp CODE_SEG:pm_init ; a:b => a is now the offset into the GDT, not a register that holds an address

[bits 32]

pm_init:
  mov ebp, STACK_BASE_PM ; 0x9fc00 - 0x1 = 0x9fbff
  mov esp, ebp
  mov ax, DATA_SEG ; all segment registers will use the data segment
  mov ds, ax
  mov es, ax
  mov ss, ax

  jmp KERNEL_START_ADDR

%include "gdt.inc"
