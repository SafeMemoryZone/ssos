#include "screen.h"

void kmain() {
  clear_screen();
  uint8_t style = 0;

  for(int i = 0; i < 500; i++) {
    print("Hello, World", style);
    style++;
  }

  print("I'm a newline\n", DEFAULT_STYLE);
}
