#include <stdint.h>
#include <kinux/mm.h>

uint32_t kmalloc(uint32_t size) {
  uint32_t t = p_addr;
  p_addr += size;
  return t;
}
