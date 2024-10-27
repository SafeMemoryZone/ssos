#include "isr.h"
#include "idt.h"
#include "screen.h"

void install_isrs() {
  idt[0] = make_idt_entry((u32)isr0);
  idt[1] = make_idt_entry((u32)isr1);
  idt[2] = make_idt_entry((u32)isr2);
  idt[3] = make_idt_entry((u32)isr3);
  idt[4] = make_idt_entry((u32)isr4);
  idt[5] = make_idt_entry((u32)isr5);
  idt[6] = make_idt_entry((u32)isr6);
  idt[7] = make_idt_entry((u32)isr7);
  idt[8] = make_idt_entry((u32)isr8);
  idt[9] = make_idt_entry((u32)isr9);
  idt[10] = make_idt_entry((u32)isr10);
  idt[11] = make_idt_entry((u32)isr11);
  idt[12] = make_idt_entry((u32)isr12);
  idt[13] = make_idt_entry((u32)isr13);
  idt[14] = make_idt_entry((u32)isr14);
  idt[15] = make_idt_entry((u32)isr15);
  idt[16] = make_idt_entry((u32)isr16);
  idt[17] = make_idt_entry((u32)isr17);
  idt[18] = make_idt_entry((u32)isr18);
  idt[19] = make_idt_entry((u32)isr19);
  idt[20] = make_idt_entry((u32)isr20);
  idt[21] = make_idt_entry((u32)isr21);
  idt[22] = make_idt_entry((u32)isr22);
  idt[23] = make_idt_entry((u32)isr23);
  idt[24] = make_idt_entry((u32)isr24);
  idt[25] = make_idt_entry((u32)isr25);
  idt[26] = make_idt_entry((u32)isr26);
  idt[27] = make_idt_entry((u32)isr27);
  idt[28] = make_idt_entry((u32)isr28);
  idt[29] = make_idt_entry((u32)isr29);
  idt[30] = make_idt_entry((u32)isr30);
  idt[31] = make_idt_entry((u32)isr31);
}

void default_isr_handler(cpu_ctx_t ctx) {
  static char *exception_messages[] = {
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

  clear_screen();
  kprint("CPU Exception: ", VGA_GET_STYLE(VGA_RED, VGA_BLACK));
  kprint(exception_messages[ctx.int_num], VGA_GET_STYLE(VGA_RED, VGA_BLACK));
  kprint("\n", VGA_DEFAULT_STYLE);
}
