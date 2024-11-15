[extern kmain]
[global _start]

section .kinit
_start:
  call kmain
  jmp $
