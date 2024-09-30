#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#include "level_events.h"
#include "all_sprites.h"
#include "event_enemy1.h"
#include "timer.h"
#include "collision.h"

#define EVENT_PERIOD 20000

struct {
  int x;
  float y;
  int width;
  int height;
  int sprite_num;
  unsigned long last_frame_clock;
} explosions[MAX_ENEMIES];
int explosions_count = 0;

LEVEL_EVENT *level_event;
ENEMY1* enemies[MAX_ENEMIES];
int enemies_count = 0;
int sprite_num = 0;
unsigned long last_enemy_clock = 0;
unsigned long start_event_clock = 0;


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

  enemies[enemies_count++] = enemy;

  add_object_to_collision_list((COL_OBJECT *)enemy);

  last_enemy_clock = game_clock_ms;
}

void create_explosion(int x, int y) {
  explosions[explosions_count].x = x + (enemy2_sprite.width[0] - enemy1_expl_sprite.width[0]) / 2;
  explosions[explosions_count].y = y;
  explosions[explosions_count].width = enemy1_expl_sprite.width[0];
  explosions[explosions_count].height = enemy1_expl_sprite.height[0];
  explosions[explosions_count].sprite_num = 0;
  explosions[explosions_count].last_frame_clock = game_clock_ms;

  explosions_count++;
}

void start_event__enemy1(LEVEL_EVENT *event) {
  level_event = event;
  start_event_clock = game_clock_ms;
}

void draw_explosions() {
  for(int i = 0; i < explosions_count; i++) {
    if (explosions[i].sprite_num >= enemy1_expl_sprite.max_sprites || explosions[i].y > SCREEN_HEIGHT) {
      explosions[i] = explosions[explosions_count - 1];
      explosions_count--;
      continue;
    }

    draw_sprite(enemy1_expl_sprite, explosions[i].sprite_num, explosions[i].x, explosions[i].y);

    if(game_clock_ms - explosions[i].last_frame_clock > 50) {
      explosions[i].sprite_num++;
      explosions[i].last_frame_clock = game_clock_ms;
    }

    explosions[i].y += 0.5;
  }
}

void draw_event__enemy1() {
  // create enemy if there are no enemies and still enough time
  if(enemies_count < MAX_ENEMIES && game_clock_ms - start_event_clock <= EVENT_PERIOD && game_clock_ms - last_enemy_clock > 500) {
    create_enemy1();
  }

  for(int i = 0; i < enemies_count; i++) {
    if (enemies[i]->base.y > SCREEN_HEIGHT) {
      remove_object_from_collision_list((COL_OBJECT *)enemies[i]);
      free(enemies[i]);
      enemies[i] = enemies[enemies_count - 1];
      enemies_count--;
      continue;
    }

    if (enemies[i]->energy <= 0) {
      create_explosion(enemies[i]->base.x, enemies[i]->base.y);
      remove_object_from_collision_list((COL_OBJECT *)enemies[i]);
      free(enemies[i]);
      enemies[i] = enemies[enemies_count - 1];
      enemies_count--;
      continue;
    }
  }

  // draw enemies
  for(int i = 0; i < enemies_count; i++) {
    ENEMY1 *enemy = enemies[i];

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
  if(game_clock_ms - start_event_clock > EVENT_PERIOD && enemies_count <= 0) {
    stop_event(level_event);
  }
}

void clear_event__enemy1() {
  for(int i = 0; i < enemies_count; i++) {
    free(enemies[i]);
  }
}

void hit(void *object) {
  ENEMY1 *enemy = object;

  enemy->energy--;
  enemy->just_hit = enemy->energy > 0 ? 1 : 0;
  enemy->last_hit_clock = game_clock_ms;
}

