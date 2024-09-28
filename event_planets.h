#ifndef EVENT_PLANETS_H
#define EVENT_PLANETS_H

#include "graphics_def.h"
#include "timer.h"
#include "level.h"
#include "level_events_def.h"
#include "all_sprites.h"

typedef struct {
  LEVEL_EVENT *level_event;
  int x;
  float y;
  int sprite_num;
  int speed;
} PLANET;

PLANET *planets[20];
int planets_count = 0;

void start_event__planet(LEVEL_EVENT *event) {
  PLANET *planet = (PLANET *)malloc(sizeof(PLANET));

  planet->level_event = event;
  planet->sprite_num = event->event_data[0];
  planet->x = event->event_data[1];
  planet->speed = event->event_data[2];
  planet->y = 0.0f - planets_sprite.height[planet->sprite_num];

  planets[planets_count++] = planet;
}

void draw_event__planet() {
  for(int i = 0; i < planets_count; i++) {
    if (planets[i]->y > SCREEN_HEIGHT) {
      stop_event(planets[i]->level_event);
      free(planets[i]);
      planets[i] = planets[planets_count - 1];
      planets_count--;
      continue;
    }
  }

  for(int i = 0; i < planets_count; i++) {
    PLANET *planet = planets[i];

    float speed = (float)(planet->speed * delta_frame_time) / (float)TICKS_PER_SECOND;

    draw_sprite(planets_sprite, planet->sprite_num, planet->x, planet->y);

    planet->y += speed;
  }

  // char str[100];
  // sprintf(str, "p: %i", planets_count);
  // draw_string(2, 0, str, 3);
}

void clear_event__planet() {
  for(int i = 0; i < planets_count; i++) {
    free(planets[i]);
  }
}

#endif
