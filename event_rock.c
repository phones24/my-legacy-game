#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#include "level.h"
#include "graphics_def.h"
#include "list.h"
#include "level_events.h"
#include "all_sprites.h"
#include "timer.h"
#include "event_rock.h"
#include "collision.h"

#define MAX_ROCKS 10
#define EVENT_PERIOD 50000
#define ROCK_FRAMES 6

static LIST *rocks;
static LEVEL_EVENT *level_event;
static unsigned long last_created_rock_clock = 0;
static unsigned long start_event_clock = 0;

void on_hit(void *object) {
  ROCK *rock = (ROCK *)object;

  rock->energy--;
  rock->just_hit = rock->energy > 0 ? 1 : 0;
  rock->last_hit_clock = game_clock_ms;
}

void create_rock() {
  ROCK *rock = (ROCK *)malloc(sizeof(ROCK));

  rock->base.x = 10 + (rand() % (SCREEN_WIDTH - 50));
  rock->base.y = 0.0f - rock_sprite.height[0];
  rock->base.width = rock_sprite.width[0];
  rock->base.height = rock_sprite.height[0];
  rock->base.hit_box_x1 = 5;
  rock->base.hit_box_y1 = 5;
  rock->base.hit_box_x2 = rock->base.width - 2;
  rock->base.hit_box_y2 = rock->base.height - 2;
  rock->sprite_num = 0;
  rock->speed = 4;
  rock->energy = 10;
  rock->last_frame_clock = 0;

  list_add(rocks, rock);

  add_object_to_collision_list((COL_OBJECT *)rock, &on_hit);

  last_created_rock_clock = game_clock_ms;
}

void init_event__rock() {
  rocks = list_create(sizeof(ROCK));
}

void start_event__rock(LEVEL_EVENT *event) {
  level_event = event;
  start_event_clock = game_clock_ms;
}

void draw_event__rock() {
  if(rocks->size < MAX_ROCKS && game_clock_ms - start_event_clock <= EVENT_PERIOD && game_clock_ms - last_created_rock_clock > 1500) {
    create_rock();
  }

  for(int i = 0; i < rocks->size; i++) {
    ROCK *rock = list_get(rocks, i);

    if(rock->base.y > SCREEN_HEIGHT) {
      remove_object_from_collision_list((COL_OBJECT *)rock);
      list_remove(rocks, i);
      continue;
    }

    if(rock->energy <= 0) {
      // create_explosion(rock->base.x, rock->base.y);
      remove_object_from_collision_list((COL_OBJECT *)rock);
      list_remove(rocks, i);
      continue;
    }
  }

  for(int i = 0; i < rocks->size; i++) {
    ROCK *rock = list_get(rocks, i);

    float speed = (float)(rock->speed * delta_frame_time) / (float)TICKS_PER_SECOND;

    draw_sprite(rock_sprite, rock->sprite_num, rock->base.x, rock->base.y, IMAGE_DRAW_MODE_NORMAL);

    if (rock->just_hit) {
      draw_sprite(
        small_expl_sprite,
        0,
        rock->base.x + ((rock_sprite.width[rock->sprite_num] - small_expl_sprite.width[0]) / 2),
        rock->base.y + rock_sprite.height[rock->sprite_num] - small_expl_sprite.height[0] - 2,
        IMAGE_DRAW_MODE_NORMAL
      );

      if(game_clock_ms - rock->last_hit_clock > 100) {
        rock->just_hit = 0;
      }
    }

    rock->base.y += speed;

    if(game_clock_ms - rock->last_frame_clock > 100) {
      rock->sprite_num++;
      rock->last_frame_clock = game_clock_ms;

      if(rock->sprite_num >= ROCK_FRAMES) {
        rock->sprite_num = 0;
      }
    }
  }

  // // draw explosions
  // draw_explosions();

  // stop event if there are no enemies and enough time has passed
  if(game_clock_ms - start_event_clock > EVENT_PERIOD && rocks->size == 0) {
    stop_event(level_event);
  }
}

void clear_event__rock() {
  for(int i = 0; i < rocks->size; i++) {
    free(list_get(rocks, i));
  }

  list_free(rocks);
}
