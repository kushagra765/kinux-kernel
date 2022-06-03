#include <kinux/io.h>
#include <stdint.h>

unsigned char inb(unsigned short port) {
  unsigned char rv;
  __asm__ volatile("inb %1, %0" : "=a"(rv) : "dN"(port));
  return rv;
}

void outb(unsigned short port, unsigned char data) {
  __asm__ volatile("outb %1, %0" : : "dN"(port), "a"(data));
}

void io_wait() {
  outb(0x80, 0);
}

uint32_t inbl(uint16_t port) {
  uint32_t rv;
  __asm__ volatile("inl %%dx, %%eax" : "=a"(rv) : "dN"(port));
  return rv;
}

void outbl(uint16_t port, uint32_t data) {
  __asm__ volatile("outl %%eax, %%dx" : : "dN"(port), "a"(data));
}
