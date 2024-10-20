org 0x7c00
bits 16

jmp _start

%include "./pm_switch.S"

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
  call _load_kernel
  call _switch_to_pm

  jmp $

; params:
;   si = pointer to null-terminated string
_print:
  pusha

print_loop:
  lodsb ; loads [ds:si] into al
  cmp al, 0
  je print_done

  mov ah, 0x0e ; display Character
  int 0x10 ; BIOS video service
  
  cmp al, 10
  jne print_continue

  ; reset column to 0 because of a newline
  mov ah, 0x03
  mov bh, 0 ; page number 0
  int 0x10 ; returns ax = 0, ch = start scan line, cl = end scan line, dh = row, dl = column

  mov dl, 0 ; reset column to 0
  mov ah, 0x02
  int 0x10 ; set cursor position

print_continue:
  jmp print_loop 

print_done:
  popa
  ret

; params:
;  none 
_load_kernel:
  pusha

  ; todo load kernel

  popa
  ret

times 510-($-$$) db 0
dw 0xaa55 
