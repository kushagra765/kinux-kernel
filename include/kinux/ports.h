#ifndef _KINUX_PORTS_H
#define _KINUX_PORTS_H

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);

#endif //_KINUX_PORTS_H
