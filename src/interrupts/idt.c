#include "idt.h"

#include <stdint.h>

#include "../misc.h"

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

typedef struct {
	uint16_t limit;
	uintptr_t base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
static idtr_t idtr;

extern void* isr_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
	idt_entry_t* descriptor = &idt[vector];

	descriptor->isr_low = (uint64_t)isr & 0xFFFF;
	descriptor->kernel_cs = GDT_OFFSET_KERNEL_CODE;
	descriptor->ist = 0;
	descriptor->attributes = flags;
	descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
	descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
	descriptor->reserved = 0;
}

void exception_handler(interrupt_frame_t *frame) {}
void irq_handler(interrupt_frame_t *frame) {}

void init_idt() {
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

	for (uint8_t vector = 0; vector < 48; vector++) {
		idt_set_descriptor(vector, isr_stub_table[vector], 0x8e);  // 0x8e = interrupt gate
	}

	__asm__ __volatile__("lidt %0" : : "m"(idtr) : "memory");
}
