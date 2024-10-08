#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#include "timer.h"
#include "midasdll.h"

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

volatile SCHEDULED_TASK task_list[MAX_TASKS];
volatile unsigned long frame_count = 0;
volatile unsigned long game_clock_ms = 0;
unsigned long begin_frame_time, delta_frame_time = 10;
volatile int on_pause = 0;

void __cdecl timer_callback(void) {
  if(!on_pause) {
    frame_count++;
    game_clock_ms += TICK_INTERVAL;

    for (int i = 0; i < MAX_TASKS; i++) {
      if (task_list[i].active && game_clock_ms >= task_list[i].time_to_execute) {
          task_list[i].func();
          task_list[i].active = 0;
      }
    }
  }
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
