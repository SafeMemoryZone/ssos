#include "idt.h"
#include "isr.h"
#include "vga_screen.h"

void init_idt() {
  install_isrs();
  load_idt();
}

void kmain() {
  clear_screen();
  init_idt();
  __asm__ __volatile__("int $2");
  __asm__ __volatile__("int $3");
  __asm__ __volatile__("int $4");
  for (;;) {
  }  // never return
}
