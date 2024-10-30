#include "idt.h"
#include "isr.h"

idt_entry_t idt[IDT_ENTRIES_COUNT];
idt_register_t idt_reg;

idt_entry_t make_idt_entry(u32 handler_addr) {
  return (idt_entry_t) {
    .low_addr = handler_addr & 0xFFFF,
    .selector = 0x8, // code segment
    .always0 = 0,
    .flags = 0x8e,
    .high_addr = handler_addr >> 16
  };
}

void load_idt() {
  idt_reg.base = (u32) &idt;
  idt_reg.limit = IDT_ENTRIES_COUNT * sizeof(idt_entry_t) - 1;

  __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}

void init_idt() {
  install_isrs();
  load_idt();
}
