#ifndef PIC_H
#define PIC_H
#include <stdint.h>

void init_PIC(void);
void enable_interrupts(void);
void PIC_sendEOI(uint8_t irq);
#endif // PIC_H
