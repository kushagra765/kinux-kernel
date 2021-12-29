#ifndef _KINUX_MM_H
#define _KINUX_MM_H

#include <stdint.h>

extern uint32_t end; // Defined in link.ld
uint32_t p_addr = (uint32_t)&end; // Placement Address

#endif //_KINUX_MM_H
