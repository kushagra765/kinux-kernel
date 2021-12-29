#ifndef _KINUX_CONSOLE_H
#define _KINUX_CONSOLE_H

#include <stdint.h>

#define COLOR_BLACK 0x0
#define COLOR_WHITE 0xF

void console_putc(uint8_t c);
void console_write(const char *fmt, ...);
void console_write_int(uint32_t num);
void console_write_hex(uint32_t num);
void init_console();

#endif //_KINUX_CONSOLE_H
