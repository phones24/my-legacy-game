#pragma once

#define TICKS_PER_SECOND 10

extern volatile unsigned long game_clock;
extern volatile unsigned long game_clock_ms;
extern unsigned long begin_frame_time, delta_frame_time;

void schedule_task(unsigned long delay, void (*func)());
void init_timer();
void restore_timer();
