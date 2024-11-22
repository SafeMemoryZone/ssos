[extern irq_handler]
[global isr0]
[global isr1]
[global isr2]
[global isr3]
[global isr4]
[global isr5]
[global isr6]
[global isr7]
[global isr8]
[global isr9]
[global isr10]
[global isr11]
[global isr12]
[global isr13]
[global isr14]
[global isr15]
[global isr16]
[global isr17]
[global isr18]
[global isr19]
[global isr20]
[global isr21]
[global isr22]
[global isr23]
[global isr24]
[global isr25]
[global isr26]
[global isr27]
[global isr28]
[global isr29]
[global isr30]
[global isr31]

isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    cld
    lea eax, [esp]
    push eax
    call irq_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8

    iret

%macro isr_with_err 1
isr%+%1:
    push %1
    jmp isr_common_stub
%endmacro

%macro isr_no_err 1
isr%+%1:
    push 0
    push %1
    jmp isr_common_stub
%endmacro

; Division Error
isr_no_err 0
; Debug
isr_no_err 1
; Non-Maskable Interrupt
isr_no_err 2
; Breakpoint
isr_no_err 3
; Overflow
isr_no_err 4
; Bound Range Exceeded
isr_no_err 5
; Invalid Opcode
isr_no_err 6
; Device Not Available
isr_no_err 7
; Double Fault
isr_with_err 8
; Coprocessor Segment Overrun (Deprecated)
isr_no_err 9
; Invalid TSS
isr_with_err 10
; Segment Not Present
isr_with_err 11
; Stack-Segment Fault
isr_with_err 12
; General Protection Fault
isr_with_err 13
; Page Fault
isr_with_err 14
; Reserved
isr_no_err 15
; x87 Floating-Point Exception
isr_no_err 16
; Alignment Check
isr_with_err 17
; Machine Check
isr_no_err 18
; SIMD Floating-Point Exception
isr_no_err 19
; Virtualization Exception
isr_no_err 20
; Control Protection Exception
isr_with_err 21
; Reserved
isr_no_err 22
; Reserved
isr_no_err 23
; Reserved
isr_no_err 24
; Reserved
isr_no_err 25
; Reserved
isr_no_err 26
; Reserved
isr_no_err 27
; Hypervisor Injection Exception
isr_no_err 28
; VMM Communication Exception
isr_with_err 29
; Security Exception
isr_with_err 30
; Reserved
isr_no_err 31
