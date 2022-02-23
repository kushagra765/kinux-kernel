#include <kinux/ports.h>

unsigned char inb(unsigned short port) {
  unsigned char rv;
  __asm__ volatile("inb %1, %0" : "=a"(rv) : "dN"(port));
  return rv;
}

void outb(unsigned short port, unsigned char data) {
  __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(data));
}
