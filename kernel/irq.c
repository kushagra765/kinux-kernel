#include <kinux/idt.h>
#include <kinux/irq.h>
#include <kinux/isr.h>
#include <kinux/io.h>

void *irq_routines[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void irq_install_handler(int n, void (*irq_handler)(struct registers *r)) {
  irq_routines[n] = irq_handler;
}

void irq_remove_handler(int n) {
  irq_routines[n] = 0;
}

void irq_remap() {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);
}

void irq_handler(struct registers *r) {
  void (*irq_handler)(struct registers * r);
  irq_handler = irq_routines[r->int_no - 32];

  if (irq_handler) {
    irq_handler(r);
  }

  if (r->int_no >= 40) {
    outb(0xA0, 0x20);
  }

  outb(0x20, 0x20);
}

void init_irq() {
  irq_remap();
  idt_set_gate(32, (uintptr_t)irq0, 0x08, 0x8E);
  idt_set_gate(33, (uintptr_t)irq1, 0x08, 0x8E);
  idt_set_gate(34, (uintptr_t)irq2, 0x08, 0x8E);
  idt_set_gate(35, (uintptr_t)irq3, 0x08, 0x8E);
  idt_set_gate(36, (uintptr_t)irq4, 0x08, 0x8E);
  idt_set_gate(37, (uintptr_t)irq5, 0x08, 0x8E);
  idt_set_gate(38, (uintptr_t)irq6, 0x08, 0x8E);
  idt_set_gate(39, (uintptr_t)irq7, 0x08, 0x8E);
  idt_set_gate(40, (uintptr_t)irq8, 0x08, 0x8E);
  idt_set_gate(41, (uintptr_t)irq9, 0x08, 0x8E);
  idt_set_gate(42, (uintptr_t)irq10, 0x08, 0x8E);
  idt_set_gate(43, (uintptr_t)irq11, 0x08, 0x8E);
  idt_set_gate(44, (uintptr_t)irq12, 0x08, 0x8E);
  idt_set_gate(45, (uintptr_t)irq13, 0x08, 0x8E);
  idt_set_gate(46, (uintptr_t)irq14, 0x08, 0x8E);
  idt_set_gate(47, (uintptr_t)irq15, 0x08, 0x8E);
}
