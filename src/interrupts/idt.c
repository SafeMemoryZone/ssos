#include "idt.h"

#include <stdint.h>

#include "drivers/screen.h"
#include "misc.h"
#include "pic.h"

#define IDT_MAX_DESCRIPTORS 256
#define GDT_OFFSET_KERNEL_CODE 0x28

// https://wiki.osdev.org/Interrupts_Tutorial
typedef struct {
	uint16_t isr_low;    // The lower 16 bits of the ISR's address
	uint16_t kernel_cs;  // The GDT segment selector that the CPU will load into CS before calling
	                     // the ISR
	uint8_t ist;         // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t attributes;  // Type and attributes; see the IDT page
	uint16_t isr_mid;    // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t isr_high;   // The higher 32 bits of the ISR's address
	uint32_t reserved;   // Set to zero
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uintptr_t base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
static idtr_t idtr;

extern void* isr_stub_table[];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void default_irq_handler(interrupt_frame_t* frame, uint64_t irq_num) {
#pragma GCC diagnostic pop
}

void (*irq_drivers[16])(interrupt_frame_t*, uint64_t) = {
    default_irq_handler, default_irq_handler, default_irq_handler, default_irq_handler,
    default_irq_handler, default_irq_handler, default_irq_handler, default_irq_handler,
    default_irq_handler, default_irq_handler, default_irq_handler, default_irq_handler,
    default_irq_handler, default_irq_handler, default_irq_handler, default_irq_handler,
};

static void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
	idt_entry_t* descriptor = &idt[vector];

	descriptor->isr_low = (uintptr_t)isr & 0xFFFF;
	descriptor->kernel_cs = GDT_OFFSET_KERNEL_CODE;
	descriptor->ist = 0;
	descriptor->attributes = flags;
	descriptor->isr_mid = ((uintptr_t)isr >> 16) & 0xFFFF;
	descriptor->isr_high = ((uintptr_t)isr >> 32) & 0xFFFFFFFF;
	descriptor->reserved = 0;
}

void exception_handler(interrupt_frame_t* frame, uint64_t int_num) {
	kprint("[ERROR] Got exception ");
	kprint_addr(int_num);
	kprint(" from address ");
	kprint_addr(frame->rip);
	kprint(" with error code ");
	kprint_addr(frame->error_code);
	kprint(".\n");
	stop();
}

void irq_handler(interrupt_frame_t* frame, uint64_t irq_num) {
	irq_drivers[irq_num](frame, irq_num);
	PIC_sendEOI(irq_num);
}

void install_irq_driver(int irq_num, void (*handler)(interrupt_frame_t*, uint64_t)) {
	irq_drivers[irq_num] = handler;
}

void init_idt() {
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

	for (uint8_t vector = 0; vector < 48; vector++) {
		idt_set_descriptor(vector, isr_stub_table[vector], 0x8e);  // 0x8e = interrupt gate
	}

	__asm__ __volatile__("lidt %0" : : "m"(idtr) : "memory");
}
