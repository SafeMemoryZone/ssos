#ifndef IDT_H
#define IDT_H
#include <stdint.h>

typedef struct {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t error_code;  // Set to 0 when it's unavailable
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__((packed)) interrupt_frame_t;

void idt_init(void);
void idt_install_irq_driver(int irq_num, void (*handler)(interrupt_frame_t*, uint64_t));
void enable_interrupts(void);
void stop(void);
void wait_for_interrupts(void);
#endif  // IDT_H
