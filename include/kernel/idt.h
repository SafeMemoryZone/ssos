#ifndef IDT_H
#define IDT_H
#include "types.h"

#define IDT_ENTRIES_COUNT 256

typedef struct {
  u16 low_addr;  // Lower 16 bits of interrupt handler
  u16 selector;  // Segment selector of handler (gdt)
  u8 always0;    // Always 0
  u8 flags;
  u16 high_addr;  // Higher 16 bits of interrupt handler
} __attribute__((packed)) idt_entry_t;

typedef struct {
  u16 limit;
  u32 base;
} __attribute__((packed)) idt_register_t;

extern idt_entry_t idt[IDT_ENTRIES_COUNT];

void init_idt();
void load_idt();
idt_entry_t make_idt_entry(u32 handler_addr);
#endif  // IDT_H
