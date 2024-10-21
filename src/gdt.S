[bits 16]

gdt_start:
  dq 0  

gdt_code:
  dw 0xffff ; limit (2 bytes)
  dw 0 ; base (2 bytes)
  db 0 ; base (1 byte)
  db 10011010b ; access byte
  db 11001111b ; limit + access flags (1 byte)
  db 0 ; base (1 byte)

gdt_data:
  dw 0xffff
  dw 0
  db 0
  db 10010010b
  db 11001111b
  db 0

gdt_end:
  
gdt_descriptor:
  dw gdt_end - gdt_start - 1 ; size (2 bytes)
  dd gdt_start ; address (4 bytes)

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
