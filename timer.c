#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#include "timer.h"

#define MAX_TASKS 10
#define PIT_CONTROL 0x43
#define PIT_CHANNEL_0 0x40
#define DIVISOR 11931

typedef void (*TASK_FUNC)();
typedef struct {
  unsigned long time_to_execute;
  TASK_FUNC func;
  char active;
} SCHEDULED_TASK;

SCHEDULED_TASK task_list[MAX_TASKS];
unsigned *bios_ticks = (unsigned *)0x0040006CL;
volatile unsigned long game_clock = 0;
volatile unsigned long game_clock_ms = 0;
unsigned long begin_frame_time, delta_frame_time = 10;

void interrupt (*old_timer_isr)(void);

void interrupt new_timer_isr(void) {
  game_clock++;
  game_clock_ms = game_clock * TICKS_PER_SECOND;

  for (int i = 0; i < MAX_TASKS; i++) {
    if (task_list[i].active && game_clock_ms >= task_list[i].time_to_execute) {
        task_list[i].func();
        task_list[i].active = 0;
    }
  }

  (*old_timer_isr)();
}

void schedule_task(unsigned long delay, TASK_FUNC func) {
  for (int i = 0; i < MAX_TASKS; i++) {
    if (!task_list[i].active) {
      task_list[i].time_to_execute = game_clock_ms + delay;
      task_list[i].func = func;
      task_list[i].active = 1;
      break;
    }
  }
}

void init_timer() {
  for (int i = 0; i < MAX_TASKS; i++) {
    task_list[i].active = 0;
  }

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
