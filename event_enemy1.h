#pragma once

#include "graphics_def.h"
#include "level_events.h"
#include "level.h"
#include "base_object.h"

#define MAX_ENEMIES 10

typedef struct {
  BASE_OBJECT base;
  float speed;
  int energy;
} ENEMY1;

extern ENEMY1* enemies[MAX_ENEMIES];
extern int enemies_count;

void hit(void *object);
void start_event__enemy1(LEVEL_EVENT *event);
void draw_event__enemy1();
void clear_event__enemy1();

