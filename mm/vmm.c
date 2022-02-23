#include <kinux/pmm.h>
#include <kinux/printm.h>
#include <kinux/vmm.h>
#include <lib/string.h>
#include <stdint.h>

uint32_t *page_directory = (uint32_t *)PAGE_DIR_VIRT_ADDR;
uint32_t *page_tables = (uint32_t *)PAGE_TABLE_VIRT_ADDR;

page_dir_t *curr_dir;

extern int paging_active;

void vmm_map(uint32_t va, uint32_t pa, uint32_t flags) {
  uint32_t virt_page = va / 0x1000;
  uint32_t pt_idx = PAGE_DIR_INDEX(virt_page);

  if (!page_directory[pt_idx]) {
    page_directory[pt_idx] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
    memset(page_tables[pt_idx * 1024], 0, 0x1000);
  }

  page_tables[virt_page] = (pa & PAGE_MASK) | flags;
}

void vmm_unmap(uint32_t va) {
  uint32_t virt_page = va / 0x1000;
  page_tables[virt_page] = 0;
  __asm__ volatile("invlpg (%0)" : : "a"(va));
}

void switch_page_dir(page_dir_t *pd) {
  curr_dir = pd;
  __asm__ volatile("mov %0, %%cr3" : : "r"(pd));
}

int vmm_get_mapping(uint32_t va, uint32_t *pa) {
  uint32_t virt_page = va / 0x1000;
  uint32_t pt_idx = PAGE_DIR_INDEX(virt_page);

  if (!page_directory[pt_idx]) {
    return 0;
  }

  if (page_tables[virt_page]) {
    if (pa) {
      *pa = page_tables[virt_page] & PAGE_MASK;
    }
    return 1;
  }
}

void init_vmm() {
  uint32_t cr0;

  page_dir_t *pd = (page_dir_t *)pmm_alloc_page();
  memset(pd, 0, 0x1000);

  pd[0] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
  uint32_t *pt = (uint32_t *)(pd[0] & PAGE_MASK);
  for (int i = 0; i < 1024; i++) {
    pt[i] = i * 0x1000 | PAGE_PRESENT | PAGE_WRITE;
  }

  pd[1022] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
  pt = (uint32_t *)(pd[1022] & PAGE_MASK);
  memset(pt, 0, 0x1000);

  pt[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_WRITE;
  pd[1023] = (uint32_t)pd | PAGE_PRESENT | PAGE_WRITE;

  switch_page_dir(pd);

  __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000;
  __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));

  uint32_t pt_idx = PAGE_DIR_INDEX((PMM_STACK_ADDR >> 12));
  page_directory[pt_idx] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
  memset(page_tables[pt_idx * 1024], 0, 0x1000);

  paging_active = 1;
  printm("vmm: initialized\n");
}
