#pragma once

#include "graphics_def.h"
#include "level_events.h"
#include "level.h"
#include "base_object.h"

typedef struct {
  BASE_OBJECT base;
  float speed;
  float position;
  float velocity;
  int energy;
  int sprite_num;
  int just_hit;
  unsigned long last_hit_clock;
} ENEMY2;

void init_event__enemy2();
// void on_hit(void *object);
void start_event__enemy2(LEVEL_EVENT *event);
void draw_event__enemy2();
void clear_event__enemy2();

