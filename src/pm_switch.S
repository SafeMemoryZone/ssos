%include "gdt.S"

STACK_BASE_PM equ 0x9fbff 

_switch_to_pm:
  cli ; disable interrupts
  lgdt [gdt_descriptor] ; load gdt
  mov eax, cr0 ; do some magic to enable pm
  or eax, 1
  mov cr0, eax
  jmp CODE_SEG:_pm_start ; a:b => a is now the offset into the GDT, not a register that hold an address

_pm_start:
  mov ax, DATA_SEG ; all segment registers will use the data segment
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax 

  mov ebp, STACK_BASE_PM ; 0x9fc00 - 0x1 = 0x9fbff
  mov esp, ebp

  jmp $ ; TODO: jump to kernel
