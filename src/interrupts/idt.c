#include "idt.h"

#include <stdint.h>

#include "drivers/screen.h"
#include "pic.h"

#define IDT_ENTRY_COUNT 256
#define KERNEL_CODE_SELECTOR 0x28

typedef struct {
	uint16_t isr_low;    // Lower 16 bits of the ISR's address
	uint16_t kernel_cs;  // GDT segment selector loaded into CS before calling the ISR
	uint8_t ist;         // IST in the TSS loaded into RSP; set to zero for now
	uint8_t attributes;  // Type and attributes; see the IDT documentation
	uint16_t isr_mid;    // Next 16 bits of the lower 32 bits of the ISR's address
	uint32_t isr_high;   // Higher 32 bits of the ISR's address
	uint32_t reserved;   // Set to zero
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uintptr_t base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(0x10))) static idt_entry_t idt_entries[IDT_ENTRY_COUNT];
static idtr_t idt_reg;

extern void* isr_stub_table[];

static void idt_default_irq_handler(interrupt_frame_t*, uint64_t);

void (*idt_irq_drivers[16])(interrupt_frame_t*, uint64_t) = {
    idt_default_irq_handler, idt_default_irq_handler, idt_default_irq_handler,
    idt_default_irq_handler, idt_default_irq_handler, idt_default_irq_handler,
    idt_default_irq_handler, idt_default_irq_handler, idt_default_irq_handler,
    idt_default_irq_handler, idt_default_irq_handler, idt_default_irq_handler,
    idt_default_irq_handler, idt_default_irq_handler, idt_default_irq_handler,
    idt_default_irq_handler,
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void idt_default_irq_handler(interrupt_frame_t* frame, uint64_t irq_num) {
	// Default IRQ handler does nothing
}
#pragma GCC diagnostic pop

static void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
	idt_entry_t* descriptor = &idt_entries[vector];

	descriptor->isr_low = (uintptr_t)isr & 0xFFFF;
	descriptor->kernel_cs = KERNEL_CODE_SELECTOR;
	descriptor->ist = 0;
	descriptor->attributes = flags;
	descriptor->isr_mid = ((uintptr_t)isr >> 16) & 0xFFFF;
	descriptor->isr_high = ((uintptr_t)isr >> 32) & 0xFFFFFFFF;
	descriptor->reserved = 0;
}

void idt_exception_handler(interrupt_frame_t* frame, uint64_t int_num) {
	screen_print("[ERROR] Got exception ");
	screen_print_addr(int_num);
	screen_print(" from address ");
	screen_print_addr(frame->rip);
	screen_print(" with error code ");
	screen_print_addr(frame->error_code);
	screen_print(".\n");
	stop();
}

void idt_irq_handler(interrupt_frame_t* frame, uint64_t irq_num) {
	idt_irq_drivers[irq_num](frame, irq_num);
	pic_send_eoi(irq_num);
}

void idt_init(void) {
	idt_reg.base = (uintptr_t)&idt_entries[0];
	idt_reg.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_ENTRY_COUNT - 1);

	for (uint8_t vector = 0; vector < 48; vector++) {
		idt_set_descriptor(vector, isr_stub_table[vector], 0x8e);  // 0x8e = interrupt gate
	}

	__asm__ __volatile__("lidt %0" : : "m"(idt_reg) : "memory");
}

void idt_install_irq_driver(int irq_num, void (*handler)(interrupt_frame_t*, uint64_t)) {
	idt_irq_drivers[irq_num] = handler;
}

void enable_interrupts(void) { __asm__ __volatile__("sti" ::: "memory"); }

__attribute__((noreturn)) void stop(void) {
	asm volatile("cli");
	while (1) {
		asm volatile("hlt");
	}
}

void wait_for_interrupts(void) { __asm__ __volatile__("hlt"); }
