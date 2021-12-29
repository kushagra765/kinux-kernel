#include "keyboard.h"
#include <kinux/irq.h>
#include <kinux/console.h>
#include <kinux/ports.h>

keymap_t us = {
  {
    /* first row - indices 0 to 14 */
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    
    /* second row - indices 15 to 28 */
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', //Enter key
    
    /* 29 = Control, 30 - 41: third row */
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    
    /* fourth row, indices 42 to 54, zeroes are shift-keys*/
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    
    '*',
    
    /* Special keys */
    
    0, //ALT - 56
    ' ', // Space - 57
    0, //Caps lock - 58
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 to F10 - 59 to 68
    0, //Num lock - 69
    0, //Scroll lock - 70
    0, //Home - 71
    0, //Up arrow - 72
    0, //Page up - 73
    '-',
    0, //Left arrow - 75
    0,
    0, //Right arrow -77
    '+',
    0, //End - 79
    0, //Dowm arrow - 80
    0, //Page down - 81
    0, //Insert - 82
    0, //Delete - 83
    0, 0, 0,
    0, //F11 - 87
    0, //F12 - 88
    0, //All others undefined
  },
  //caps
  {
    /* first row - indices 0 to 14 */
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    
    /* second row - indices 15 to 28 */
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', //Enter key
    
    /* 29 = Control, 30 - 41: third row */
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    
    /* fourth row, indices 42 to 54, zeroes are shift-keys*/
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    
    '*',
    
    /* Special keys */
    
    0, //ALT - 56
    ' ', // Space - 57
    0, //Caps lock - 58
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 to F10 - 59 to 68
    0, //Num lock - 69
    0, //Scroll lock - 70
    0, //Home - 71
    0, //Up arrow - 72
    0, //Page up - 73
    '-',
    0, //Left arrow - 75
    0,
    0, //Right arrow -77
    '+',
    0, //End - 79
    0, //Dowm arrow - 80
    0, //Page down - 81
    0, //Insert - 82
    0, //Delete - 83
    0, 0, 0,
    0, //F11 - 87
    0, //F12 - 88
    0, //All others undefined
  },
  
  // control_map
  {
    29, // Ctrl
    56, // Alt
    0,  // AltGr
    42, // left Shift
    54, // right Shift
    58, // Caps lock
    70, // Scroll lock
    69  // Num lock
  },
  0
};

keymap_t *current_layout;
uint32_t kb_buf_start, kb_buf_end;
char kb_buf[256];

char keyboard_getchar() {
  if (kb_buf_start != kb_buf_end) {
      char c = kb_buf[kb_buf_start++];
      kb_buf_end &= 255;
      return c;
  } else {
      return '\0';
  }
}

void keyboard_handler() {
  uint8_t scancode = inb(0x60);
  
  if(scancode & RELEASED_MASK) {
    for(uint32_t i = 0; i < 5; i++) {
      if(current_layout->control_map[i] == (scancode & ~RELEASED_MASK)) {
        current_layout->controls &= ~(1 << i);
        return;
      }
    }
  } else {
    for (uint32_t i = 0; i < 8; i++) {
      if(current_layout->control_map[i] == scancode) {
        if(current_layout->controls & 1 << i) {
          current_layout->controls &= ~(1 << i);
        } else {
          current_layout->controls |= 1 << i;
        }
        return;
      }
    }

    uint8_t *scancodes = current_layout->scancodes;
    if ((current_layout->controls & (LSHIFT | RSHIFT | CAPSLOCK))
       && !(current_layout->controls & CONTROL)) {
        scancodes = current_layout->shift_scancodes;
    }

    if (kb_buf_end != kb_buf_start - 1) {
        kb_buf[kb_buf_end++] = scancodes[scancode];
        kb_buf_end &= 255;
    }
  }
}

void keyboard_idle() {
  for (;;) {
    char c = keyboard_getchar();
    if (c) {
      console_putc(c);
    }
  }
}

void init_keyboard() {
  irq_install_handler(1, &keyboard_handler);
  current_layout = &us;
  kb_buf_start = 0;
  kb_buf_end = 0;
}
