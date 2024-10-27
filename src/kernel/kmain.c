#include "screen.h"
#include "isr.h"
#include "idt.h"

void init_idt() {
  install_isrs();
  load_idt();
}

void kmain() {
  clear_screen();
  init_idt();

  for(;;) {} // never return
}
