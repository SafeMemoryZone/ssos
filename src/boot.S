[org 0x7c00]
[bits 16]

STACK_BASE equ 0x8000
KERNEL_START_ADDR equ 0x1000
BOOT_DRIVE_NUMBER equ 0x80

start:
  cli
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, STACK_BASE
  sti

  mov dl, BOOT_DRIVE_NUMBER
  call load_kernel
  call switch_to_pm

  jmp $

  ; params:
  ;  dl - boot drive number
load_kernel:
  pusha

  mov ah, 0x02 ; 0x02 = 'read'
  mov al, 16 ; 16 sectors
  mov cl, 2 ; start sector
  xor ch, ch ; cylinder
  xor dh, dh ; head
  mov bx, KERNEL_START_ADDR ; es:bx pointer to store buffer

  int 0x13

  popa
  ret

%include "prot_mode.inc"
times 510-($-$$) db 0
dw 0xaa55
