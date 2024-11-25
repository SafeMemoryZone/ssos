[extern kmain]
[extern kernel_stack]
[global _start]

KERNEL_STACK_SIZE equ 4096
DATA_SEG equ 0x10

section .kinit

_start:
  mov ax, DATA_SEG
  mov ds, ax
  mov es, ax
  mov ss, ax
  lea ebp, [kernel_stack + KERNEL_STACK_SIZE] 
  mov esp, ebp

  cld
  call kmain

  jmp $
