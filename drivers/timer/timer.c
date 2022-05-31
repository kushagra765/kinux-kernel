#include <kinux/io.h>
#include <kinux/irq.h>
#include <kinux/printm.h>
#include <kinux/timer.h>
#include <stdint.h>

int timer_ticks;

static void timer_phase(int hz) {
  int divisor = 1193180 / hz;
  outb(0x43, 0x36);
  outb(0x40, divisor & 0xFF);
  outb(0x40, divisor >> 8);
}

void timer_handler() {
  timer_ticks++;
}

void timer_sleep(int ticks) {
  uint16_t eticks;
  eticks = timer_ticks + ticks;
  while (timer_ticks < eticks)
    ;
}

void init_timer(int hz) {
  timer_ticks = 0;
  irq_install_handler(0, timer_handler);
  timer_phase(hz);
  printm("pit: initialized. hz=%d\n", hz);
}
