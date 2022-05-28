#include <kinux/console.h>
#include <kinux/io.h>
#include <lib/stdarg.h>
#include <lib/string.h>
#include <stdint.h>

uint16_t *text_ptr;
int attrib = 0x0F;
int cur_x = 0, cur_y = 0;

void console_scroll() {
  unsigned blank, tmp;
  blank = 0x20 | (attrib << 8);

  if (cur_y >= 25) {
    tmp = cur_y - 25 + 1;
    memcpy(text_ptr, text_ptr + tmp * 80, (25 - tmp) * 80 * 2);
    memsetw(text_ptr + (25 - tmp) * 80, blank, 80);
    cur_y = 25 - 1;
  }
}

void console_move_cur() {
  unsigned tmp;
  tmp = cur_y * 80 + cur_x;
  outb(0x3D4, 14);
  outb(0x3D5, tmp >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, tmp);
}

void console_clear() {
  unsigned blank;
  blank = 0x20 | (attrib << 8);
  for (int i = 0; i < 25; i++) {
    memsetw(text_ptr + i * 80, blank, 80);
  }
  cur_y = cur_x = 0;
  console_move_cur();
}

void console_putc(uint8_t c) {
  uint16_t *where;
  unsigned attr = attrib << 8;

  if (c == 0x08 && cur_x != 0) {
    cur_x--;
  } else if (c == 0x09) {
    cur_x = (cur_x + 8) & ~(8 - 1);
  } else if (c == '\r') {
    cur_x = 0;
  } else if (c == '\n') {
    cur_x = 0;
    cur_y++;
  } else if (c >= ' ') {
    where = text_ptr + (cur_y * 80 + cur_x);
    *where = c | attr;
    cur_x++;
  }

  if (cur_x >= 80) {
    cur_x = 0;
    cur_y++;
  }

  console_scroll();
  console_move_cur();
}

void console_write(const char *fmt, ...) {
  va_list ap;
  char *p, *sval;
  int ival, xval;

  va_start(ap, fmt);

  for (p = fmt; *p; p++) {
    if (*p != '%') {
      console_putc(*p);
      continue;
    }

    switch (*++p) {
    case 's':
      for (sval = va_arg(ap, char *); *sval; sval++) {
        console_putc(*sval);
      }
      break;
    case 'd':
      ival = va_arg(ap, int);
      console_write_int(ival);
      break;
    case 'x':
      xval = va_arg(ap, int);
      console_write_hex(xval);
      break;
    default:
      console_putc(*p);
      break;
    }
  }

  va_end(ap);
}

/* console_write_int() and console_write_hex()
 * are taken from JamesM's kernel development tutorials
 */
void console_write_int(uint32_t num) {
  int a = num;
  int i = 0;
  char c[30];

  if (num == 0) {
    console_putc('0');
    return;
  }

  while (a > 0) {
    c[i] = '0' + a % 10;
    a /= 10;
    i++;
  }

  c[i] = 0;

  char c2[30];
  c2[i--] = 0;
  int j = 0;

  while (i >= 0) {
    c2[i--] = c[j++];
  }

  console_write(c2);
}

void console_write_hex(uint32_t num) {
  signed int t;

  console_write("0x");

  int no_zeroes = 1;

  for (int i = 28; i > 0; i -= 4) {
    t = (num >> i) & 0xF;
    if (t == 0 && no_zeroes != 0) {
      continue;
    }

    if (t >= 0xA) {
      no_zeroes = 0;
      console_putc(t - 0xA + 'a');
    } else {
      no_zeroes = 0;
      console_putc(t + '0');
    }
  }

  t = num & 0xF;
  if (t >= 0xA) {
    console_putc(t - 0xA + 'a');
  } else {
    console_putc(t + '0');
  }
}

void init_console() {
  text_ptr = (uint16_t *)0xB8000;
  console_clear();
}
