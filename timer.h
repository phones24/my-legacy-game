#pragma once

#define TICKS_PER_SECOND 100
#define TICK_INTERVAL 10

extern volatile unsigned long frame_count;
extern volatile unsigned long game_clock_ms;
extern unsigned long begin_frame_time, delta_frame_time;
extern volatile int on_pause;

void schedule_task(unsigned long delay, void (*func)());
void __cdecl timer_callback();

