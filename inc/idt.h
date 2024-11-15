#ifndef IDT_H
#define IDT_H
#include <stdint.h>

#define IDT_ENTRIES_COUNT 256

typedef struct {
  uint16_t low_addr;  // Lower 16 bits of interrupt handler
  uint16_t selector;  // Segment selector of handler (gdt)
  uint8_t always0;    // Always 0
  uint8_t flags;
  uint16_t high_addr;  // Higher 16 bits of interrupt handler
} __attribute__((packed)) idt_entry_t;

typedef struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idt_register_t;

extern idt_entry_t idt[IDT_ENTRIES_COUNT];

void init_idt();
void load_idt();
idt_entry_t make_idt_entry(uintptr_t handler_addr);
#endif  // IDT_H
