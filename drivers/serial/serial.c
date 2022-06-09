#include <kinux/io.h>
#include <kinux/printm.h>
#include <kinux/serial.h>
#include <lib/string.h>

#define SERIAL_COM1 0x3f8

int serial_received() {
  return inb(SERIAL_COM1 + 5) & 1;
}

char serial_read() {
  while (serial_received() == 0)
    ;
  return inb(SERIAL_COM1);
}

int serial_is_transmit_empty() {
  return inb(SERIAL_COM1 + 5) & 0x20;
}

void serial_write(char c) {
  while (serial_is_transmit_empty() == 0)
    ;
  outb(SERIAL_COM1, c);
}

void serial_writestr(char *str) {
  for (int i = 0; i < strlen(str); i++) {
    serial_write(str[i]);
  }
}

int init_serial() {
  outb(SERIAL_COM1 + 1, 0x00);
  outb(SERIAL_COM1 + 3, 0x80);
  outb(SERIAL_COM1 + 0, 0x03);
  outb(SERIAL_COM1 + 1, 0x00);
  outb(SERIAL_COM1 + 3, 0x03);
  outb(SERIAL_COM1 + 2, 0xC7);
  outb(SERIAL_COM1 + 4, 0x0B);
  outb(SERIAL_COM1 + 4, 0x1E);
  outb(SERIAL_COM1 + 0, 0xAE);

  if (inb(SERIAL_COM1 + 0) != 0xAE) {
    printm("serial: warning: faulty\n");
  }

  outb(SERIAL_COM1 + 4, 0x0F);
  return 0;
}
