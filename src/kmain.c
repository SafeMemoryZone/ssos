#include <stddef.h>
#include <stdint.h>

#include "idt.h"
#include "vga_screen.h"

#define KERNEL_STACK_SIZE 4096
__attribute__((aligned(16))) uint8_t kernel_stack[KERNEL_STACK_SIZE];

void kmain() {
  clear_screen();
  init_idt();
  __asm__ __volatile__("int $2");
  for (;;);
}
