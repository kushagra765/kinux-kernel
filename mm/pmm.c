#include <kinux/panic.h>
#include <kinux/pmm.h>
#include <kinux/printm.h>
#include <kinux/vmm.h>
#include <stdint.h>

uint32_t pmm_stack_loc = PMM_STACK_ADDR;
uint32_t pmm_stack_max = PMM_STACK_ADDR;
uint32_t pmm_loc;
int paging_active = 0;

uint32_t pmm_alloc_page() {
  if (paging_active) {
    if (pmm_stack_loc == PMM_STACK_ADDR) {
      panic("PMM: Out of memory");
    }
    pmm_stack_loc -= sizeof(uint32_t);
    uint32_t *stack = (uint32_t *)pmm_stack_loc;
    return *stack;
  } else {
    return pmm_loc += 0x1000;
  }
}

void pmm_free_page(uint32_t page) {
  if (page < pmm_loc) {
    return;
  }

  if (pmm_stack_max <= pmm_stack_loc) {
    vmm_map(pmm_stack_max, page, PAGE_PRESENT | PAGE_WRITE);
    pmm_stack_max += PMM_PAGE_SZ;
  } else {
    uint32_t *stack = (uint32_t *)pmm_stack_loc;
    *stack = page;
    pmm_stack_loc += sizeof(uint32_t);
  }
}

void pmm_free_available_pages(multiboot_info_t *info) {
  uint32_t i = info->mmap_addr;
  while (i < info->mmap_addr + info->mmap_length) {
    multiboot_memory_map_t *entry = (multiboot_memory_map_t *)i;
    if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
      for (uint32_t j = entry->base_addr_low;
           j < entry->base_addr_low + entry->length_low; j += 0x1000) {
        pmm_free_page(j);
      }
    }
    i += entry->size + sizeof(uint32_t);
  }
}

void init_pmm(uint32_t start) {
  pmm_loc = (start + 0x1000) & PAGE_MASK;
  printm("pmm: initialized. pmm_loc=0x%x\n", pmm_loc);
}
