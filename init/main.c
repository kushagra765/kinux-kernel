#include "multiboot.h"
#include <fs/tarfs/tarfs.h>
#include <generated/buildinfo.h>
#include <keyboard/keyboard.h>
#include <kinux/cmdline.h>
#include <kinux/console.h>
#include <kinux/cpuid.h>
#include <kinux/gdt.h>
#include <kinux/ide.h>
#include <kinux/idt.h>
#include <kinux/irq.h>
#include <kinux/isr.h>
#include <kinux/kheap.h>
#include <kinux/pmm.h>
#include <kinux/printm.h>
#include <kinux/serial.h>
#include <kinux/timer.h>
#include <kinux/vmm.h>

void start_kernel(multiboot_info_t *mboot_info) {
  init_console();
  printm("%s\n", BUILD_INFO);
  printm("console: %dx%d\n", mboot_info->framebuffer_width,
         mboot_info->framebuffer_height);
  init_gdt();
  init_idt();
  init_isr();
  init_irq();
  printm("gdt, idt, isr and irq initialized\n");
  cpuid_print_info();
  uint32_t total_mem = (mboot_info->mem_lower + mboot_info->mem_upper) / 1024;
  printm("Total memory %d MB\n", total_mem);
  init_pmm(mboot_info->mem_upper);
  init_vmm();
  pmm_free_available_pages(mboot_info);
  init_kheap();
  __asm__ volatile("sti");
  init_serial();
  cmdline_parse((const char *)mboot_info->cmdline);
  init_timer(100);
  init_keyboard();
  uint32_t initrd = *((uint32_t *)mboot_info->mods_addr);
  init_tarfs(initrd);
  tarfs_list_headers();
  init_ide(0x1f0, 0x3f6, 0x170, 0x376, 0x000);
  console_write("Hello, World!\n");
  keyboard_idle();
}
