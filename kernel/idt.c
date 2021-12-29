#include <stdint.h>
#include <kinux/idt.h>
#include <lib/string.h>

struct idt_entry {
  uint16_t lo_base;
  uint16_t sel;
  uint8_t always0;
  uint8_t flags;
  uint16_t hi_base;
} __attribute__((packed));

struct idt_pointer {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_pointer idt_ptr;

void idt_set_gate(uint8_t n, uint32_t base, uint16_t sel, uint8_t flags) {
  idt[n].lo_base = (base & 0xFFFF);
  idt[n].sel = sel;
  idt[n].always0 = 0;
  idt[n].flags = flags;
  idt[n].hi_base = (base >> 16) & 0xFFFF;
}

void init_idt() {
  idt_ptr.limit = (sizeof(struct idt_entry) * 256) - 1;
  idt_ptr.base = (uintptr_t)&idt;
  memset(&idt, 0, sizeof(struct idt_entry) * 256);
  load_idt();
}
