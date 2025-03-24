#include "pic.h"

#include "drivers/ports.h"

#define PIC_MASTER 0x20  // Master PIC
#define PIC_SLAVE 0xA0   // Slave PIC
#define PIC_MASTER_COMMAND PIC_MASTER
#define PIC_MASTER_DATA (PIC_MASTER + 1)
#define PIC_SLAVE_COMMAND PIC_SLAVE
#define PIC_SLAVE_DATA (PIC_SLAVE + 1)

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

static void pic_remap(int master_offset, int slave_offset) {
	outb(PIC_MASTER_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC_SLAVE_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC_MASTER_DATA, master_offset);  // ICW2: Master PIC
	io_wait();
	outb(PIC_SLAVE_DATA, slave_offset);  // ICW2: Slave PIC
	io_wait();
	outb(PIC_MASTER_DATA, 4);  // ICW3: tell Master PIC that there is a slave PIC at IRQ2
	io_wait();
	outb(PIC_SLAVE_DATA, 2);  // ICW3: tell Slave PIC its cascade identity
	io_wait();

	outb(PIC_MASTER_DATA, ICW4_8086);  // ICW4: have the PICs use 8086 mode
	io_wait();
	outb(PIC_SLAVE_DATA, ICW4_8086);
	io_wait();

	// Unmask both PICs
	outb(PIC_MASTER_DATA, 0);
	outb(PIC_SLAVE_DATA, 0);
}

void pic_init(void) { pic_remap(0x20, 0x28); }

void pic_send_eoi(uint8_t irq) {
	if (irq >= 8) outb(PIC_SLAVE_COMMAND, PIC_EOI);
	outb(PIC_MASTER_COMMAND, PIC_EOI);
}
