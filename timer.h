#ifndef TIMER_H
#define TIMER_H

#define TICKS_PER_SECOND 18.2
// #define MS_PER_TICK (1000 / TICKS_PER_SECOND)
#define MS_PER_TICK 10
#define PIT_CONTROL 0x43
#define PIT_CHANNEL_0 0x40
#define DIVISOR 11931

volatile static unsigned long game_clock = 0;
volatile static unsigned long game_clock_ms = 0;

unsigned *bios_ticks = (unsigned *)0x0040006CL;

unsigned long get_time_ms() {
  unsigned long ticks = *bios_ticks;

  return (unsigned long)(ticks * MS_PER_TICK);
}

void interrupt (*old_timer_isr)(void);

void interrupt new_timer_isr(void) {
    game_clock++;
    game_clock_ms = game_clock * MS_PER_TICK;
    (*old_timer_isr)();
}

void init_timer() {
  old_timer_isr = _dos_getvect(0x1C);

  _dos_setvect(0x1C, new_timer_isr);

  outp(PIT_CONTROL, 0x36);
  outp(PIT_CHANNEL_0, DIVISOR & 0xFF);
  outp(PIT_CHANNEL_0, DIVISOR >> 8);
}

void restore_timer() {
  _dos_setvect(0x1C, old_timer_isr);

  outp(PIT_CONTROL, 0x36);
  outp(PIT_CHANNEL_0, 0);
  outp(PIT_CHANNEL_0, 0);
}

#endif
