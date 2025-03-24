#include "gpt.h"

#include "drivers/disk.h"
#include "drivers/screen.h"
#include "mem/mem.h"
#include "misc/bit_math.h"

static uint64_t backup_lba;
static uint64_t first_usable_lba;
static uint64_t last_usable_lba;
static uint8_t disk_guid[16];
static uint64_t partition_entries_lba;
static uint32_t num_partition_entries;
static uint32_t partition_entry_size;
static uint32_t crc32_partition_array;

int gpt_init(void) {
	uint8_t *gpt_header = kmalloc(512);

	if (!gpt_header) {
		return -1;
	}

	if (disk_read(1, 1, gpt_header)) {
		kfree(gpt_header);
		return -2;
	}

	uint64_t signature;
	uint32_t gpt_revision;
	uint32_t header_size;
	uint32_t crc32_header;
	uint64_t curr_lba;

	memcpy(&signature, gpt_header + 0x00, sizeof(uint64_t));
	memcpy(&gpt_revision, gpt_header + 0x08, sizeof(uint32_t));
	memcpy(&header_size, gpt_header + 0x0C, sizeof(uint32_t));
	memcpy(&crc32_header, gpt_header + 0x10, sizeof(uint32_t));
	memset(gpt_header + 0x10, 0, sizeof(uint32_t));
	memcpy(&curr_lba, gpt_header + 0x18, sizeof(uint64_t));
	memcpy(&backup_lba, gpt_header + 0x20, sizeof(uint64_t));
	memcpy(&first_usable_lba, gpt_header + 0x28, sizeof(uint64_t));
	memcpy(&last_usable_lba, gpt_header + 0x30, sizeof(uint64_t));
	memcpy(disk_guid, gpt_header + 0x38, 16);
	memcpy(&partition_entries_lba, gpt_header + 0x48, sizeof(uint64_t));
	memcpy(&num_partition_entries, gpt_header + 0x50, sizeof(uint32_t));
	memcpy(&partition_entry_size, gpt_header + 0x54, sizeof(uint32_t));
	memcpy(&crc32_partition_array, gpt_header + 0x58, sizeof(uint32_t));

	// "EFI PART" signature
	if (signature != 0x5452415020494645ULL) {
		kfree(gpt_header);
		return -3;
	}

	if (gpt_revision != 0x00010000UL) {
		kfree(gpt_header);
		return -4;  // Unknown revision
	}

	if (header_size != 92) {
		kfree(gpt_header);
		return -5;  // Unknown fields in the header
	}

	if (calculate_ccitt32_ansi_crc32(gpt_header, header_size) != crc32_header) {
		kfree(gpt_header);
		return -6;
	}

	if (curr_lba != 1) {
		kfree(gpt_header);
		return -7;
	}

	kfree(gpt_header);
	return 0;
}
