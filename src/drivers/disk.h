#ifndef DISK_H
#define DISK_H
#include <stdint.h>

int disk_init(void);
int disk_read(uint32_t start_lba, uint16_t sector_count, void *buff);
int disk_write(uint32_t start_lba, uint16_t sector_count, const void *buff);
#endif  // DISK_H
