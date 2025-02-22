#ifndef PORTS_H
#define PORTS_H
#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);
void io_wait(void);
#endif  // PORTS_H
