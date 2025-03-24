#include "disk.h"

#include <stddef.h>
#include <stdint.h>

#include "interrupts/idt.h"
#include "ports.h"

#define ATA_PRIMARY_BUS_BASE 0x1F0
#define ATA_PRIMARY_BUS_CTRL_BASE 0x3F6

#define ATA_REG_DATA (ATA_PRIMARY_BUS_BASE + 0)
#define ATA_REG_ERR (ATA_PRIMARY_BUS_BASE + 1)
#define ATA_REG_FEATURES (ATA_PRIMARY_BUS_BASE + 1)
#define ATA_REG_SECTOR_COUNT (ATA_PRIMARY_BUS_BASE + 2)
#define ATA_REG_LBA_LOW (ATA_PRIMARY_BUS_BASE + 3)
#define ATA_REG_LBA_MID (ATA_PRIMARY_BUS_BASE + 4)
#define ATA_REG_LBA_HIGH (ATA_PRIMARY_BUS_BASE + 5)
#define ATA_REG_DRIVE_HEAD (ATA_PRIMARY_BUS_BASE + 6)
#define ATA_REG_STATUS (ATA_PRIMARY_BUS_BASE + 7)
#define ATA_REG_COMMAND (ATA_PRIMARY_BUS_BASE + 7)
#define ATA_REG_ALT_STATUS (ATA_PRIMARY_BUS_CTRL_BASE + 0)
#define ATA_REG_DEVICE_CTRL (ATA_PRIMARY_BUS_CTRL_BASE + 0)
#define ATA_REG_DRIVE_ADDR (ATA_PRIMARY_BUS_CTRL_BASE + 1)

#define ATA_COMMAND_IDENTIFY 0xEC
#define ATA_COMMAND_READ_SECTORS 0x20
#define ATA_COMMAND_WRITE_SECTORS 0x30
#define ATA_COMMAND_CACHE_FLUSH 0xE7

#define ATA_STATUS_BSY (1 << 7)
#define ATA_STATUS_DRQ (1 << 3)
#define ATA_STATUS_ERR (1 << 0)
#define ATA_STATUS_DF (1 << 5)

static uint16_t identify_data_block[256];

static void disk_irq_handler(interrupt_frame_t *frame, uint64_t irq_num) {
	// TODO: implement irq handler (for async)
}

int disk_init(void) {
	// Select master drive
	outb(ATA_REG_DRIVE_HEAD, 0xA0);

	// 400ns delay
	inb(ATA_REG_ALT_STATUS);
	inb(ATA_REG_ALT_STATUS);
	inb(ATA_REG_ALT_STATUS);
	inb(ATA_REG_ALT_STATUS);

	// Clear sector count and lba registers
	outb(ATA_REG_SECTOR_COUNT, 0);
	outb(ATA_REG_LBA_LOW, 0);
	outb(ATA_REG_LBA_MID, 0);
	outb(ATA_REG_LBA_HIGH, 0);

	// Send the IDENTIFY command
	outb(ATA_REG_COMMAND, ATA_COMMAND_IDENTIFY);

	uint8_t status = inb(ATA_REG_STATUS);
	if (status == 0) {
		return -1;  // No drive present
	}

	// Wait for BSY to clear
	while (inb(ATA_REG_STATUS) & ATA_STATUS_BSY);

	// It's not an ATA drive if any of these change
	if (inb(ATA_REG_LBA_MID) || inb(ATA_REG_LBA_HIGH)) {
		return -2;
	}

	// Poll until DRQ is not set
	while (!(inb(ATA_REG_STATUS) & ATA_STATUS_DRQ)) {
		if (inb(ATA_REG_STATUS) & ATA_STATUS_ERR) {
			return -3;
		}
	}

	for (int i = 0; i < 256; i++) {
		identify_data_block[i] = inw(ATA_REG_DATA);
	}

	idt_install_irq_driver(14, disk_irq_handler);
	return 0;
}

int disk_read(uint32_t start_lba, uint16_t sector_count, void *buff) {
	// Ensure drive is not busy
	while (inb(ATA_REG_STATUS) & ATA_STATUS_BSY);

	// Set drive to master and set upper 4 bits of lba
	outb(ATA_REG_DRIVE_HEAD, 0xE0 | ((start_lba >> 24) & 0x0F));

	// 400ns delay
	inb(ATA_REG_ALT_STATUS);
	inb(ATA_REG_ALT_STATUS);
	inb(ATA_REG_ALT_STATUS);
	inb(ATA_REG_ALT_STATUS);

	outb(ATA_REG_SECTOR_COUNT, sector_count >= 256 ? 0 : sector_count);
	outb(ATA_REG_LBA_LOW, (uint8_t)(start_lba & 0xFF));
	outb(ATA_REG_LBA_MID, (uint8_t)((start_lba >> 8) & 0xFF));
	outb(ATA_REG_LBA_HIGH, (uint8_t)((start_lba >> 16) & 0xFF));

	// Send READ SECTORS command
	outb(ATA_REG_COMMAND, ATA_COMMAND_READ_SECTORS);

	for (size_t s = 0; s < sector_count; s++) {
		// BSY must be clear and DRQ set
		for (;;) {
			uint8_t status = inb(ATA_REG_STATUS);
			if (status & ATA_STATUS_ERR) {
				return -1;
			}
			if (!(status & ATA_STATUS_BSY) && (status & ATA_STATUS_DRQ)) {
				break;
			}
		}

		for (int i = 0; i < 256; i++) {
			uint16_t *buff_cast = buff;
			buff_cast[s * 256 + i] = inw(ATA_REG_DATA);
		}

		// 400ns delay after each sector read
		inb(ATA_REG_ALT_STATUS);
		inb(ATA_REG_ALT_STATUS);
		inb(ATA_REG_ALT_STATUS);
		inb(ATA_REG_ALT_STATUS);
	}

	return 0;
}

int disk_write(uint32_t start_lba, uint16_t sector_count, const void *buff) {
	// Ensure drive is not busy
	while (inb(ATA_REG_STATUS) & ATA_STATUS_BSY);

	// Set drive to master and set upper 4 bits of lba
	outb(ATA_REG_DRIVE_HEAD, 0xE0 | ((start_lba >> 24) & 0x0F));

	// 400ns delay
	inb(ATA_REG_ALT_STATUS);
	inb(ATA_REG_ALT_STATUS);
	inb(ATA_REG_ALT_STATUS);
	inb(ATA_REG_ALT_STATUS);

	outb(ATA_REG_SECTOR_COUNT, sector_count >= 256 ? 0 : sector_count);
	outb(ATA_REG_LBA_LOW, (uint8_t)(start_lba & 0xFF));
	outb(ATA_REG_LBA_MID, (uint8_t)((start_lba >> 8) & 0xFF));
	outb(ATA_REG_LBA_HIGH, (uint8_t)((start_lba >> 16) & 0xFF));

	// Send the WRITE SECTORS command
	outb(ATA_REG_COMMAND, ATA_COMMAND_WRITE_SECTORS);

	uint16_t *buff_cast = (uint16_t *)buff;

	// Write each sector
	for (size_t s = 0; s < sector_count; s++) {
		for (;;) {
			uint8_t status = inb(ATA_REG_STATUS);
			if (status & ATA_STATUS_ERR) {
				return -1;
			}
			if (!(status & ATA_STATUS_BSY) && (status & ATA_STATUS_DRQ)) {
				break;
			}
		}

		// Write one sector
		for (int i = 0; i < 256; i++) {
			outw(ATA_REG_DATA, buff_cast[s * 256 + i]);
			// Tiny delay
			inb(ATA_REG_ALT_STATUS);
		}

		// 400ns after each write
		inb(ATA_REG_ALT_STATUS);
		inb(ATA_REG_ALT_STATUS);
		inb(ATA_REG_ALT_STATUS);
		inb(ATA_REG_ALT_STATUS);
	}

	// After writing all sectors, flush cache
	outb(ATA_REG_COMMAND, ATA_COMMAND_CACHE_FLUSH);
	// Wait for the flush to complete
	while (inb(ATA_REG_STATUS) & ATA_STATUS_BSY);

	return 0;
}
