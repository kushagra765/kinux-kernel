#include <kinux/panic.h>
#include <kinux/console.h>

void panic(const char *str) {
  __asm__ volatile("cli");
  console_write("Kernel Panic: %s\n", str);
  __asm__ volatile("hlt");
}
