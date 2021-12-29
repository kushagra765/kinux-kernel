#ifndef _DRIVERS_KEYBOARD_H
#define _DRIVERS_KEYBOARD_H

#include <stdint.h>

#define CONTROL 0x1
#define ALT 0x2
#define ALTGR 0x4
#define LSHIFT 0x8
#define RSHIFT 0x10
#define CAPSLOCK 0x20
#define SCROLLLOCK 0x40
#define NUMLOCK 0x80

#define RELEASED_MASK 0x80

typedef uint8_t key_status_t;

typedef struct keymap {
  uint8_t scancodes[128];
  uint8_t shift_scancodes[128];
  uint8_t control_map[8];
  key_status_t controls;
} keymap_t;

void init_keyboard();
void keyboard_handler();
char keyboard_getchar();
void keyboard_idle();

#endif //_DRIVERS_KEYBOARD_H
