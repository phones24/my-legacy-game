#ifndef EVENT_NEBULA_H
#define EVENT_NEBULA_H

#include "sprites.h"

void init_event__nebula(LEVEL_EVENT *event) {
}

void start_event__nebula(LEVEL_EVENT *event) {
}

void draw_event__nebula(LEVEL_EVENT *event) {
  LEVEL_EVENT__NEBULA *cur_evevnt = (LEVEL_EVENT__NEBULA *)event->event_data;

  if (cur_evevnt->y > SCREEN_HEIGHT) {
    event->status = EVENT_STATUS_FINISHED;
    return;
  }

  draw_sprite(nebula_sprite, cur_evevnt->sprite_num, cur_evevnt->x, cur_evevnt->y);

  cur_evevnt->y += 0.1;
}

void clear_event__nebula(LEVEL_EVENT *event) {
}

#endif
