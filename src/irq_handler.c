#include "irq_handler.h"
#include "idt.h"
#include "vga_screen.h"

void install_irq_handlers() {
  idt[0] = make_idt_entry((uintptr_t)isr0);
  idt[1] = make_idt_entry((uintptr_t)isr1);
  idt[2] = make_idt_entry((uintptr_t)isr2);
  idt[3] = make_idt_entry((uintptr_t)isr3);
  idt[4] = make_idt_entry((uintptr_t)isr4);
  idt[5] = make_idt_entry((uintptr_t)isr5);
  idt[6] = make_idt_entry((uintptr_t)isr6);
  idt[7] = make_idt_entry((uintptr_t)isr7);
  idt[8] = make_idt_entry((uintptr_t)isr8);
  idt[9] = make_idt_entry((uintptr_t)isr9);
  idt[10] = make_idt_entry((uintptr_t)isr10);
  idt[11] = make_idt_entry((uintptr_t)isr11);
  idt[12] = make_idt_entry((uintptr_t)isr12);
  idt[13] = make_idt_entry((uintptr_t)isr13);
  idt[14] = make_idt_entry((uintptr_t)isr14);
  idt[15] = make_idt_entry((uintptr_t)isr15);
  idt[16] = make_idt_entry((uintptr_t)isr16);
  idt[17] = make_idt_entry((uintptr_t)isr17);
  idt[18] = make_idt_entry((uintptr_t)isr18);
  idt[19] = make_idt_entry((uintptr_t)isr19);
  idt[20] = make_idt_entry((uintptr_t)isr20);
  idt[21] = make_idt_entry((uintptr_t)isr21);
  idt[22] = make_idt_entry((uintptr_t)isr22);
  idt[23] = make_idt_entry((uintptr_t)isr23);
  idt[24] = make_idt_entry((uintptr_t)isr24);
  idt[25] = make_idt_entry((uintptr_t)isr25);
  idt[26] = make_idt_entry((uintptr_t)isr26);
  idt[27] = make_idt_entry((uintptr_t)isr27);
  idt[28] = make_idt_entry((uintptr_t)isr28);
  idt[29] = make_idt_entry((uintptr_t)isr29);
  idt[30] = make_idt_entry((uintptr_t)isr30);
  idt[31] = make_idt_entry((uintptr_t)isr31);
}

char *exception_messages[] = {
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault",
  "Unknown Interrupt",
  "Coprocessor Fault",
  "Alignment Check",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};


void irq_handler(interrupt_frame_t *frame) {
  kprint("CPU Exception: ", VGA_GET_STYLE(VGA_RED, VGA_BLACK));
  kprint(exception_messages[frame->int_num], VGA_GET_STYLE(VGA_RED, VGA_BLACK));
  kprint(" (", VGA_GET_STYLE(VGA_RED, VGA_BLACK));
  kprint_dec(frame->err_code, VGA_GET_STYLE(VGA_RED, VGA_BLACK));
  kprint(")", VGA_GET_STYLE(VGA_RED, VGA_BLACK));
  kprint("\n", VGA_DEFAULT_STYLE);
}
