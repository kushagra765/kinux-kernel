#ifndef _KINUX_CONSOLE_H
#define _KINUX_CONSOLE_H

#include <stdint.h>

#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN 0x6
#define COLOR_LIGHT_GRAY 0x7
#define COLOR_GRAY 0x8
#define COLOR_LIGHT_BLUE 0x9
#define COLOR_LIGHT_GREEN 0xA
#define COLOR_LIGHT_CYAN 0xB
#define COLOR_LIGHT_RED 0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_LIGHT_YELLOW 0xE
#define COLOR_WHITE 0xF

void console_putc(uint8_t c);
void console_write(const char *fmt, ...);
void console_write_int(uint32_t num);
void console_write_hex(uint32_t num);
void init_console();

#endif //_KINUX_CONSOLE_H
