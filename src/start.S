[org 0x7c00]
[bits 16]

jmp start

BOOTLOADER_MSG: db "[DEBUG] Bootloader loaded", 10, 0

start:
  mov bp, STACK_BASE
  mov sp, bp
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov ss, ax

  mov si, BOOTLOADER_MSG
  call print
  mov dl, BOOT_DRIVE_NUMBER
  call load_kernel
  call switch_to_pm

  jmp $

; params:
;   si = pointer to null-terminated string
print:
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
  ; TODO: check if succeeded

  popa
  ret

%include "./pm_switch.S"

times 510-($-$$) db 0
dw 0xaa55
