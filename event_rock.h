#pragma once

#include "level_events.h"
#include "base_object.h"

typedef struct {
  BASE_OBJECT base;
  float speed;
  int energy;
  int just_hit;
  int sprite_num;
  unsigned long last_hit_clock;
  unsigned long last_frame_clock;
} ROCK;

void init_event__rock();
void start_event__rock(LEVEL_EVENT *event);
void draw_event__rock();
void clear_event__rock();
void hit(void *object);


