#ifndef _KINUX_SERIAL_H
#define _KINUX_SERIAL_H

int serial_received();
char serial_read();
int serial_is_transmit_empty();
void serial_write(char c);
void serial_writestr(char *str);
int init_serial();

#endif //_KINUX_SERIAL_H
