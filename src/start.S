org 0x7c00
bits 16

STACK_BASE: equ 0x8000

jmp _start

BOOTLOADER_MSG: db "[DEBUG] Bootloader loaded", 10, 0

_start:
  mov bp, STACK_BASE
  mov sp, bp
  mov ax, 0
  mov cs, ax
  mov ds, ax
  mov es, ax
  mov ss, ax

  mov si, BOOTLOADER_MSG
  call _print

  jmp $

; params:
;   SI = pointer to null-terminated string
_print:
  pusha

print_loop:
  lodsb ; loads [DS:SI] into AL
  cmp al, 0
  je print_done

  mov ah, 0x0e ; Display Character
  int 0x10 ; BIOS video service
  
  cmp al, 10
print_continue:
  jmp print_loop 

print_done:
  popa
  ret

times 510-($-$$) db 0
dw 0xaa55 
