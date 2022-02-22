#include <kinux/console.h>
#include <kinux/gdt.h>
#include <kinux/idt.h>
#include <kinux/isr.h>
#include <kinux/irq.h>
#include <kinux/timer.h>
#include <kinux/pmm.h>
#include <kinux/vmm.h>
#include <keyboard/keyboard.h>
#include "multiboot.h"

void start_kernel(multiboot_info_t *mboot_info) {
  init_console();
  init_gdt();
  init_idt();
  init_isr();
  init_irq();
  init_pmm(mboot_info->mem_upper);
  init_vmm();
  pmm_free_available_pages(mboot_info);
  __asm__ volatile("sti");
  init_timer();
  init_keyboard();
  console_write("Hello, World!\n");
  keyboard_idle();
}
