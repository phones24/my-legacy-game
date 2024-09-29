#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#include "event_planets.h"
#include "event_enemy1.h"
#include "set.h"

#define MAX_CURRENT_EVENTS 10

int active_event_count = 0;
int next_event_num = 0;
LEVEL_EVENT *current_events[MAX_CURRENT_EVENTS];
SET* drawn_event_types;

void draw_event_of_type(LEVEL_EVENT_TYPE type) {
  switch (type) {
    case EVENT_TYPE_PLANET:
      draw_event__planet();
      break;
    case EVENT_TYPE_ENEMY1:
      draw_event__enemy1();
      break;
    default:
      break;
  }
}

void start_event(LEVEL_EVENT *event) {
  switch (event->type) {
    case EVENT_TYPE_PLANET:
      start_event__planet(event);
      break;
    case EVENT_TYPE_ENEMY1:
      start_event__enemy1(event);
      break;
    default:
      break;
  }
}

void clear_event_of_type(LEVEL_EVENT_TYPE type) {
  switch (type) {
    case EVENT_TYPE_PLANET:
      clear_event__planet();
      break;
    case EVENT_TYPE_ENEMY1:
      clear_event__enemy1();
      break;
    default:
      break;
  }
}

void init_level() {
  drawn_event_types = set_create();

  for (int i = 0; i < level_inline.count; i++) {
    level_inline.events[i].id = i;
    level_inline.events[i].status = EVENT_STATUS_NOT_STARTED;
  }
}

void check_active_events() {
  for (int i = next_event_num; i < level_inline.count && i < next_event_num + MAX_CURRENT_EVENTS; i++) {
    LEVEL_EVENT *event = &level_inline.events[i];

    if (event->status != EVENT_STATUS_NOT_STARTED) {
      continue;
    }

    if (game_clock_ms >= event->clock) {
      event->status = EVENT_STATUS_ACTIVE;
      next_event_num = i + 1;
      current_events[active_event_count++] = event;
      start_event(event);
    }
  }
}

void clear_finished_events() {
  for (int i = 0; i < active_event_count; i++) {
    if (current_events[i]->status == EVENT_STATUS_FINISHED) {
      current_events[i] = current_events[active_event_count - 1];
      active_event_count--;
      break;
    }
  }
}

void stop_event(LEVEL_EVENT *event) {
  event->status = EVENT_STATUS_FINISHED;
}

void draw_level() {
  check_active_events();

  set_clear(drawn_event_types);

  for (int i = 0; i < active_event_count; i++) {
    if (set_has(drawn_event_types, current_events[i]->type)) {
      continue;
    }

    draw_event_of_type(current_events[i]->type);

    set_add(drawn_event_types, current_events[i]->type);
  }

  clear_finished_events();

  // char str[100];
  // sprintf(str, "ae: %i", active_event_count);
  // draw_string(2, 0, str, 3);
}

void clear_level_data() {
  set_clear(drawn_event_types);

  for (int i = 0; i < level_inline.count; i++) {
    LEVEL_EVENT *event = &level_inline.events[i];

    if (set_has(drawn_event_types, event->type)) {
      continue;
    }

    clear_event_of_type(event->type);

    set_add(drawn_event_types, event->type);
  }

  set_free(drawn_event_types);
}


