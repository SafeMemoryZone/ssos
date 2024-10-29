#include "strlib.h"

size_t strlen(char *str) {
  size_t len = 0;
  while (*str++) len++;
  return len;
}

void reverse_str(char *str) {
  size_t len = strlen(str);
  size_t last_idx = len - 1;

  for (size_t i = 0; i < len / 2; i++) {
    char tmp = str[i];
    str[i] = str[last_idx];
    str[last_idx] = tmp;
    last_idx--;
  }
}
