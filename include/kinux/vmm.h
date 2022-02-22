#ifndef _KINUX_VMM_H
#define _KINUX_VMM_H

#include <stdint.h>

#define PAGE_DIR_VIRT_ADDR 0xFFBFF000
#define PAGE_TABLE_VIRT_ADDR 0xFFC00000
#define PAGE_DIR_INDEX(x) ((uint32_t)x / 1024)
#define PAGE_TABLE_INDEX(x) ((uint32_t)x % 1024)

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4
#define PAGE_MASK 0xFFFFF000

typedef uint32_t page_dir_t;

void vmm_map(uint32_t va, uint32_t pa, uint32_t flags);
void vmm_unmap(uint32_t va);
int vmm_get_mapping(uint32_t va, uint32_t *pa);
void switch_page_dir(page_dir_t *pd);
void init_vmm();

#endif //_KINUX_VMM_H
