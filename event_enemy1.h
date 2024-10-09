#pragma once

#include "graphics_def.h"
#include "level_events.h"
#include "level.h"
#include "base_object.h"

#define MAX_ENEMIES 5

typedef struct {
  BASE_OBJECT base;
  float speed;
  int energy;
  int sprite_num;
  int just_hit;
  unsigned long last_hit_clock;
} ENEMY1;

void init_event__enemy1();
void on_hit(void *object);
void start_event__enemy1(LEVEL_EVENT *event);
void draw_event__enemy1();
void clear_event__enemy1();

