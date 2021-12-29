#include <kinux/console.h>
#include <kinux/gdt.h>
#include <kinux/idt.h>
#include <kinux/isr.h>
#include <kinux/irq.h>
#include <kinux/timer.h>
#include <keyboard/keyboard.h>

void start_kernel() {
  init_console();
  init_gdt();
  init_idt();
  init_isr();
  init_irq();
  __asm__ volatile("sti");
  init_timer();
  init_keyboard();
  console_write("Hello, World!\n");
  keyboard_idle();
}
