#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#include "graphics_def.h"
#include "level_events_def.h"
#include "level.h"
#include "all_sprites.h"
#include "event_enemy1.h"
#include "timer.h"

#define EVENT_PERIOD 20000

LEVEL_EVENT *level_event;
ENEMY1* enemies[MAX_ENEMIES];
int enemies_count = 0;
int sprite_num = 0;
unsigned long start_event_clock = 0;


void create_enemy1() {
  ENEMY1 *enemy = (ENEMY1 *)malloc(sizeof(ENEMY1));

  enemy->x = 10 + (rand() % (SCREEN_WIDTH - 50));;
  enemy->y = 0.0f - enemy1_sprite.height[sprite_num];
  enemy->width = enemy1_sprite.width[sprite_num];
  enemy->height = enemy1_sprite.height[sprite_num];
  enemy->speed = 8;
  enemy->energy = 2;

  enemies[enemies_count++] = enemy;
}

void start_event__enemy1(LEVEL_EVENT *event) {
  level_event = event;
  start_event_clock = game_clock_ms;
}

void draw_event__enemy1() {
  // create enemy if there is no enemies and still enough time
  if(enemies_count < MAX_ENEMIES && game_clock_ms - start_event_clock <= EVENT_PERIOD && game_clock_ms % 500 == 0) {
    create_enemy1();
  }

  for(int i = 0; i < enemies_count; i++) {
    if (enemies[i]->y > SCREEN_HEIGHT) {
      free(enemies[i]);
      enemies[i] = enemies[enemies_count - 1];
      enemies_count--;
      continue;
    }

    if (enemies[i]->energy <= 0) {
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

    draw_sprite(enemy1_sprite, sprite_num, enemy->x, enemy->y);

    enemy->y += speed;
  }

  // stop event if there are no enemies and enough time has passed
  if(game_clock_ms - start_event_clock > EVENT_PERIOD && enemies_count <= 0) {
    stop_event(level_event);
  }

  // char str[100];
  // sprintf(str, "y: %i", enemies_count > 0 ? (int)enemies[enemies_count - 1]->y : -1);
  // draw_string(2, 0, str, 3);
  // sprintf(str, "ms: %lu", game_clock_ms);
  // draw_string(2, 8, str, 3);
}

void clear_event__enemy1() {
  for(int i = 0; i < enemies_count; i++) {
    free(enemies[i]);
  }
}
