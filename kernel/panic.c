#include <kinux/panic.h>
#include <kinux/printm.h>

void panic(const char *str) {
  __asm__ volatile("cli");
  printm("Kernel panic - %s\n", str);
  __asm__ volatile("hlt");
}
