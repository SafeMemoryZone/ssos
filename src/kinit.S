[extern kmain]
[extern kernel_stack]
[global _start]

KERNEL_STACK_SIZE equ 4096

section .kinit

_start:
  lea ebp, [kernel_stack + KERNEL_STACK_SIZE] 
  mov esp, ebp
  cld
  call kmain
  jmp $
