#ifndef _KINUX_PMM_H
#define _KINUX_PMM_H

#include <init/multiboot.h>
#include <stdint.h>

#define PMM_STACK_ADDR 0xFF000000
#define PMM_PAGE_SZ 4096

uint32_t pmm_alloc_page();
void pmm_free_page(uint32_t page);
void init_pmm(uint32_t start);
void pmm_free_available_pages(multiboot_info_t *info);

#endif //_KINUX_PMM_H
