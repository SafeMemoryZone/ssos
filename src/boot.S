[org 0x7c00]
[bits 16]

STACK_BASE equ 0x8000
KERNEL_START_ADDR equ 0x1000
BOOT_DRIVE_NUMBER equ 0x80

start:
  cli ; disable interrupts until idt is setup
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, STACK_BASE

  mov dl, BOOT_DRIVE_NUMBER
  call load_kernel
  call switch_to_pm

  jmp $

load_kernel:
  pusha

  mov ah, 0x02
  mov al, 16
  mov cl, 2
  xor ch, ch
  xor dh, dh
  mov bx, KERNEL_START_ADDR

  int 0x13

  popa
  ret

%include "prot_mode.inc"
times 510-($-$$) db 0
dw 0xaa55
