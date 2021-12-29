#include <kinux/gdt.h>
#include <kinux/console.h>
#include <stdint.h>

struct gdt_entry {
  uint16_t low_limit;
  uint16_t low_base;
  uint8_t middle_base;
  uint8_t access;
  uint8_t granularity;
  uint8_t high_base;
} __attribute__((packed));

struct gdt_pointer {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_pointer gdt_ptr;

void gdt_set_gate(int n, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
   gdt[n].low_base = (base & 0xFFFF);
   gdt[n].middle_base = (base >> 16) & 0xFF;
   gdt[n].high_base = (base >> 24) & 0xFF;
   gdt[n].low_limit = (limit & 0xFFFF);
   gdt[n].granularity = ((limit >> 16) & 0x0F);
   gdt[n].granularity |= (gran & 0xF0);
   gdt[n].access = access;
}

void init_gdt() {
  gdt_ptr.limit = (sizeof(struct gdt_entry) * 3) - 1;
  gdt_ptr.base = (uintptr_t)&gdt;
  
  gdt_set_gate(0, 0, 0, 0, 0);
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
  gdt_flush();
}
