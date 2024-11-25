#include "idt.h"

#include <stdint.h>

#include "irq_handler.h"

#define CODE_SEG 0x8

__attribute__((aligned(16))) idt_entry_t idt[IDT_ENTRIES_COUNT];
idt_register_t idt_reg;

idt_entry_t make_idt_entry(uintptr_t irq_handler_addr) {
  return (idt_entry_t){.low_addr = irq_handler_addr & 0xFFFF,
                       .selector = CODE_SEG,
                       .always0 = 0,
                       .flags = 0x8e,
                       .high_addr = irq_handler_addr >> 16};
}

void load_idt() {
  idt_reg.base = (uintptr_t)&idt;
  idt_reg.limit = IDT_ENTRIES_COUNT * sizeof(idt_entry_t) - 1;

  __asm__ __volatile__("lidtl (%0)" : : "r"(&idt_reg));
  __asm__ __volatile__("sti");
}

void init_idt() {
  install_irq_handlers();
  load_idt();
}
