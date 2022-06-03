#ifndef _KINUX_IO_H
#define _KINUX_IO_H

#include <stdint.h>

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
void io_wait();
uint32_t inbl(uint16_t port);
void outbl(uint16_t port, uint32_t data);

#endif //_KINUX_IO_H
