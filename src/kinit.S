[extern kmain]
[extern kernel_stack]
[extern __bss_start]
[extern __bss_sized]
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

  ; Zero out the .bss section a DWORD (4 bytes) at a time
  xor eax, eax
  mov edi, __bss_start
  mov ecx, __bss_sized
  rep stosd

  call kmain

  jmp $
