#ifndef EVENT_PLANETS_H
#define EVENT_PLANETS_H

#include "sprites.h"

void init_event__planet(LEVEL_EVENT *event) {
}

void start_event__planet(LEVEL_EVENT *event) {
  LEVEL_EVENT__PLANET *cur_evevnt = (LEVEL_EVENT__PLANET *)event->event_data;

  cur_evevnt->y = 0 - planets_sprite.height[cur_evevnt->sprite_num];
}

void draw_event__planet(LEVEL_EVENT *event) {
  LEVEL_EVENT__PLANET *cur_event = (LEVEL_EVENT__PLANET *)event->event_data;

  if (cur_event->y > SCREEN_HEIGHT) {
    event->status = EVENT_STATUS_FINISHED;
    return;
  }

  int speed = (cur_event->speed * delta_frame_time) / TICKS_PER_SECOND;

  draw_sprite(planets_sprite, cur_event->sprite_num, cur_event->x, cur_event->y);

  cur_event->y += speed;
}

void clear_event__planet(LEVEL_EVENT *event) {
}

#endif
