#ifndef _KINUX_CPUID_H
#define _KINUX_CPUID_H

#include <stdint.h>

void __cpuid(uint32_t type, uint32_t *eax, uint32_t *ebx, uint32_t *ecx,
             uint32_t *edx);
void cpuid_print_info();

#endif //_KINUX_CPUID_H
