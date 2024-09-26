#ifndef EVENT_ENEMY1_H
#define EVENT_ENEMY1_H

#include "graphics_def.h"
#include "level_events_def.h"
#include "level.h"

void init_event__enemy1(LEVEL_EVENT *event) {
}

void start_event__enemy1(LEVEL_EVENT *event) {
  LEVEL_EVENT__ENEMY1 *cur_evevnt = (LEVEL_EVENT__ENEMY1 *)event->event_data;

  // cur_evevnt->y = 0.0f - planets_sprite.height[cur_evevnt->sprite_num];
}

void draw_event__enemy1(LEVEL_EVENT *event) {
  // LEVEL_EVENT__ENEMY1 *cur_event = (LEVEL_EVENT__ENEMY1 *)event->event_data;

  // if (cur_event->y > SCREEN_HEIGHT) {
  //   stop_event(event);
  //   return;
  // }

  // float speed = (float)(cur_event->speed * delta_frame_time) / (float)TICKS_PER_SECOND;

  // draw_sprite(planets_sprite, cur_event->sprite_num, cur_event->x, cur_event->y);

  // cur_event->y += speed;
}

void clear_event__enemy1(LEVEL_EVENT *event) {
}

#endif
