    [extern default_isr_handler]

isr_common_stub:
    pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax, ds ; Lower 16-bits of eax = ds.
    push eax ; save the data segment descriptor
    mov ax, 0x10  ; kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call default_isr_handler

    pop eax 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8 ; Cleans up the pushed error code and pushed ISR number
    sti
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

    ; Divide By Zero Exception
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

    ; Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

    ; Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

    ; Int 3 Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

    ; INTO Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

    ; Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

    ; Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

    ; Coprocessor Not Available Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

    ; Double Fault Exception (With Error Code!)
isr8:
    cli
    push byte 8
    jmp isr_common_stub

    ; Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

    ; Bad TSS Exception (With Error Code!)
isr10:
    cli
    push byte 10
    jmp isr_common_stub

    ; Segment Not Present Exception (With Error Code!)
isr11:
    cli
    push byte 11
    jmp isr_common_stub

    ; Stack Fault Exception (With Error Code!)
isr12:
    cli
    push byte 12
    jmp isr_common_stub

    ; General Protection Fault Exception (With Error Code!)
isr13:
    cli
    push byte 13
    jmp isr_common_stub

    ; Page Fault Exception (With Error Code!)
isr14:
    cli
    push byte 14
    jmp isr_common_stub

    ; Reserved Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

    ; Floating Point Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

    ; Alignment Check Exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

    ; Machine Check Exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

    ; Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

    ; Reserved
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

    ; Reserved
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

    ; Reserved
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

    ; Reserved
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

    ; Reserved
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

    ; Reserved
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

    ; Reserved
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

    ; Reserved
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

    ; Reserved
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

    ; Reserved
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

    ; Reserved
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

    ; Reserved
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub
