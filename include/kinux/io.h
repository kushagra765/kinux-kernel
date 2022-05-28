#ifndef _KINUX_IO_H
#define _KINUX_IO_H

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);
void io_wait();

#endif //_KINUX_IO_H
