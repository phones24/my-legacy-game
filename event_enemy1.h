#ifndef EVENT_ENEMY1_H
#define EVENT_ENEMY1_H

#include "graphics_def.h"
#include "level_events_def.h"
#include "level.h"

#define MAX_ENEMIES 20

typedef struct {
  int x;
  float y;
  int width;
  int height;
  float speed;
  int energy;
} ENEMY1;

extern ENEMY1* enemies[MAX_ENEMIES];
extern int enemies_count;

void start_event__enemy1(LEVEL_EVENT *event);
void draw_event__enemy1();
void clear_event__enemy1();

#endif
