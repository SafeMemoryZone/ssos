#include "pic.h"

#include "../drivers/ports.h"

#define PIC1 0x20  // Master PIC
#define PIC2 0xA0  // Slave PIC
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM 0x10

#define PIC_EOI 0x20  // End-of-interrupt command code

static void PIC_remap(int master_off, int slave_off) {
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, master_off);  // ICW2: Master PIC
	io_wait();
	outb(PIC2_DATA, slave_off);  // ICW2: Slave PIC
	io_wait();
	outb(PIC1_DATA, 4);  // ICW3: tell Master PIC that there is a slave PIC at IRQ2
	io_wait();
	outb(PIC2_DATA, 2);  // ICW3: tell Slave PIC its cascade identity
	io_wait();

	outb(PIC1_DATA, ICW4_8086);  // ICW4: have the PICs use 8086 mode
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	// Unmask both PICs
	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 0);
}

void PIC_sendEOI(uint8_t irq) {
	if (irq >= 8) outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

void init_PIC(void) { PIC_remap(0x20, 0x28); }

void enable_interrupts(void) { __asm__ __volatile__("sti" ::: "memory"); }
