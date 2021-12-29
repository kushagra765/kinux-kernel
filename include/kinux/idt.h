#ifndef _KINUX_IDT_H
#define _KINUX_IDT_H

#include <stdint.h>

void init_idt();
extern void load_idt();
void idt_set_gate(uint8_t n, uint32_t base, uint16_t sel, uint8_t flags);

#endif //_KINUX_IDT_H
