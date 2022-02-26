#include <kinux/idt.h>
#include <kinux/isr.h>
#include <kinux/panic.h>
#include <kinux/vmm.h>
#include <stdint.h>

const char *exceptions[] = {"Division by zero",
                            "Debug",
                            "Non Maskable Interrupt",
                            "Breakpoint exception",
                            "Into Detected Overflow",
                            "Out of Bounds exception",
                            "Invalid Opcode exception",
                            "No Coprocessor exception",
                            "Double Fault exception",
                            "Coprocessor Segment Overrun",
                            "Bad TSS exception",
                            "Segment Not Present",
                            "Stack Fault exception",
                            "General Protection Fault",
                            "Page Fault exception",
                            "Unknown Interrupt exception",
                            "Coprocessor Fault exception",
                            "Alignment Check exception",
                            "Machine Check exception",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved",
                            "Reserved"};

void isr_handler(struct registers *r) {
  if (r->int_no == 14) {
    page_fault(r);
  }

  if (r->int_no <= 31) {
    panic(exceptions[r->int_no]);
  }
}

void init_isr() {
  idt_set_gate(0, (uintptr_t)isr0, 0x08, 0x8E);
  idt_set_gate(1, (uintptr_t)isr1, 0x08, 0x8E);
  idt_set_gate(2, (uintptr_t)isr2, 0x08, 0x8E);
  idt_set_gate(3, (uintptr_t)isr3, 0x08, 0x8E);
  idt_set_gate(4, (uintptr_t)isr4, 0x08, 0x8E);
  idt_set_gate(5, (uintptr_t)isr5, 0x08, 0x8E);
  idt_set_gate(6, (uintptr_t)isr6, 0x08, 0x8E);
  idt_set_gate(7, (uintptr_t)isr7, 0x08, 0x8E);
  idt_set_gate(8, (uintptr_t)isr8, 0x08, 0x8E);
  idt_set_gate(9, (uintptr_t)isr9, 0x08, 0x8E);
  idt_set_gate(10, (uintptr_t)isr10, 0x08, 0x8E);
  idt_set_gate(11, (uintptr_t)isr11, 0x08, 0x8E);
  idt_set_gate(12, (uintptr_t)isr12, 0x08, 0x8E);
  idt_set_gate(13, (uintptr_t)isr13, 0x08, 0x8E);
  idt_set_gate(14, (uintptr_t)isr14, 0x08, 0x8E);
  idt_set_gate(15, (uintptr_t)isr15, 0x08, 0x8E);
  idt_set_gate(16, (uintptr_t)isr16, 0x08, 0x8E);
  idt_set_gate(17, (uintptr_t)isr17, 0x08, 0x8E);
  idt_set_gate(18, (uintptr_t)isr18, 0x08, 0x8E);
  idt_set_gate(19, (uintptr_t)isr19, 0x08, 0x8E);
  idt_set_gate(20, (uintptr_t)isr20, 0x08, 0x8E);
  idt_set_gate(21, (uintptr_t)isr21, 0x08, 0x8E);
  idt_set_gate(22, (uintptr_t)isr22, 0x08, 0x8E);
  idt_set_gate(23, (uintptr_t)isr23, 0x08, 0x8E);
  idt_set_gate(24, (uintptr_t)isr24, 0x08, 0x8E);
  idt_set_gate(25, (uintptr_t)isr25, 0x08, 0x8E);
  idt_set_gate(26, (uintptr_t)isr26, 0x08, 0x8E);
  idt_set_gate(27, (uintptr_t)isr27, 0x08, 0x8E);
  idt_set_gate(28, (uintptr_t)isr28, 0x08, 0x8E);
  idt_set_gate(29, (uintptr_t)isr29, 0x08, 0x8E);
  idt_set_gate(30, (uintptr_t)isr30, 0x08, 0x8E);
  idt_set_gate(31, (uintptr_t)isr31, 0x08, 0x8E);
}
