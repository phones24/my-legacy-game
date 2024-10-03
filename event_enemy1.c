#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#include "level_events.h"
#include "all_sprites.h"
#include "event_enemy1.h"
#include "timer.h"
#include "collision.h"
#include "list.h"

#define EVENT_PERIOD 50000

typedef struct {
  int x;
  float y;
  int width;
  int height;
  int sprite_num;
  unsigned long last_frame_clock;
} EXPLOSION;


LEVEL_EVENT *level_event;
LIST *enemies_list;
LIST *explosions;
int sprite_num = 0;
unsigned long last_enemy_clock = 0;
unsigned long start_event_clock = 0;

void init_event__enemy1() {
  enemies_list = list_create();
  explosions = list_create();
}

void create_enemy1() {
  ENEMY1 *enemy = (ENEMY1 *)malloc(sizeof(ENEMY1));

  enemy->base.id = rand() % 100000000;
  enemy->base.x = 10 + (rand() % (SCREEN_WIDTH - 50));
  enemy->base.y = 0.0f - enemy2_sprite.height[sprite_num];
  enemy->base.width = enemy2_sprite.width[sprite_num];
  enemy->base.height = enemy2_sprite.height[sprite_num];
  enemy->base.hit = hit;
  enemy->speed = 8;
  enemy->energy = 2;

  list_add(enemies_list, enemy);

  add_object_to_collision_list((COL_OBJECT *)enemy);

  last_enemy_clock = game_clock_ms;
}

void create_explosion(int x, int y) {
  EXPLOSION *explosion = malloc(sizeof(EXPLOSION));

  explosion->x = x + (enemy2_sprite.width[0] - enemy1_expl_sprite.width[0]) / 2;
  explosion->y = y;
  explosion->width = enemy1_expl_sprite.width[0];
  explosion->height = enemy1_expl_sprite.height[0];
  explosion->sprite_num = 0;
  explosion->last_frame_clock = game_clock_ms;

  list_add(explosions, explosion);
}

void start_event__enemy1(LEVEL_EVENT *event) {
  level_event = event;
  start_event_clock = game_clock_ms;
}

void draw_explosions() {
  for(int i = 0; i < explosions->size; i++) {
    EXPLOSION *explosion = list_get(explosions, i);
    if (explosion->sprite_num >= enemy1_expl_sprite.max_sprites || explosion->y > SCREEN_HEIGHT) {
      list_remove(explosions, i);
      continue;
    }

    draw_sprite(enemy1_expl_sprite, explosion->sprite_num, explosion->x, explosion->y);

    if(game_clock_ms - explosion->last_frame_clock > 50) {
      explosion->sprite_num++;
      explosion->last_frame_clock = game_clock_ms;
    }

    explosion->y += 0.5;
  }
}

void draw_event__enemy1() {
  // create enemy if there are no enemies and still enough time
  if(enemies_list->size < MAX_ENEMIES && game_clock_ms - start_event_clock <= EVENT_PERIOD && game_clock_ms - last_enemy_clock > 500) {
    create_enemy1();
  }

  for(int i = 0; i < enemies_list->size; i++) {
    ENEMY1 *enemy = list_get(enemies_list, i);

    if(enemy->base.y > SCREEN_HEIGHT) {
      remove_object_from_collision_list((COL_OBJECT *)enemy);
      list_remove(enemies_list, i);
      continue;
    }

    if(enemy->energy <= 0) {
      create_explosion(enemy->base.x, enemy->base.y);
      remove_object_from_collision_list((COL_OBJECT *)enemy);
      list_remove(enemies_list, i);
      continue;
    }
  }

  // draw enemies
  for(int i = 0; i < enemies_list->size; i++) {
    ENEMY1 *enemy = list_get(enemies_list, i);

    float speed = (float)(enemy->speed * delta_frame_time) / (float)TICKS_PER_SECOND;

    draw_sprite(enemy2_sprite, sprite_num, enemy->base.x, enemy->base.y);

    if (enemy->just_hit) {
      draw_sprite(
        small_expl_sprite,
        0,
        enemy->base.x + ((enemy2_sprite.width[sprite_num] - small_expl_sprite.width[0]) / 2) + 2,
        enemy->base.y + (enemy2_sprite.height[sprite_num] / 2) - 3
      );

      if(game_clock_ms - enemy->last_hit_clock > 100) {
        enemy->just_hit = 0;
      }
    }

    enemy->base.y += speed;
  }

  // draw explosions
  draw_explosions();

  // stop event if there are no enemies and enough time has passed
  if(game_clock_ms - start_event_clock > EVENT_PERIOD && enemies_list->size == 0) {
    stop_event(level_event);
  }
}

void hit(void *object) {
  ENEMY1 *enemy = object;

  enemy->energy--;
  enemy->just_hit = enemy->energy > 0 ? 1 : 0;
  enemy->last_hit_clock = game_clock_ms;
}

void clear_event__enemy1() {
  for(int i = 0; i < enemies_list->size; i++) {
    ENEMY1 *enemy = list_get(enemies_list, i);
    free(enemy);
  }

  for(int i = 0; i < explosions->size; i++) {
    EXPLOSION *explosion = list_get(explosions, i);
    free(explosion);
  }

  list_free(explosions);
  list_free(enemies_list);
}

