#ifndef ISR_H
#define ISR_H
#include <stdint.h>

// Handlers for CPU exceptions
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

typedef struct {
  uint32_t user_ds;
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t ignored_val;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint32_t int_num;
  uint32_t err_code;
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
} interrupt_frame_t;

void install_irq_handlers();
#endif  // ISR_H
