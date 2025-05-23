#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <math.h>

#include "level_events.h"
#include "all_sprites.h"
#include "event_enemy2.h"
#include "timer.h"
#include "collision.h"
#include "list.h"
#include "sound.h"
#include "ship.h"

#define EVENT_PERIOD 10000
#define MAX_ENEMIES 2

typedef struct {
  int x;
  float y;
  int width;
  int height;
  int sprite_num;
  unsigned long last_frame_clock;
} EXPLOSION;

typedef struct {
  BASE_OBJECT base;
  int sprite_num;
  unsigned long last_frame_clock;
} PROJECTILE;

static LEVEL_EVENT *level_event;
static LIST *enemies_list;
static LIST *explosions;
static LIST *projectiles;
static unsigned long last_enemy_clock = 0;
static unsigned long start_event_clock = 0;
static int last_direction = -1;

static void on_hit(void *object) {
  ENEMY2 *enemy = object;

  enemy->energy--;
  enemy->just_hit = enemy->energy > 0 ? 1 : 0;
  enemy->last_hit_clock = game_clock_ms;
}

static void on_projectile_hit(void *object) {
  PROJECTILE *projectile = object;

}

void init_event__enemy2() {
  enemies_list = list_create();
  explosions = list_create();
  projectiles = list_create();
}

static void create_projectile(ENEMY2 *enemy) {
  PROJECTILE *projectile = malloc(sizeof(PROJECTILE));

  if (projectile == NULL) {
    fprintf(stderr, "Cannot allocate memory for projectile");
    exit(1);
  }

  projectile->base.x = enemy->base.x + enemy->base.width / 2;
  projectile->base.y = enemy->base.y + enemy->base.height / 2;
  projectile->base.width = en2_prj.width[0];
  projectile->base.height = en2_prj.height[0];
  projectile->base.hit_box_x1 = 2;
  projectile->base.hit_box_y1 = 2;
  projectile->base.hit_box_x2 = projectile->base.width - 2;
  projectile->base.hit_box_y2 = projectile->base.height - 2;
  projectile->sprite_num = 0;
  projectile->last_frame_clock = game_clock_ms;

  list_add(projectiles, projectile);
  add_object_to_collision_list((COL_OBJECT *)projectile, &on_projectile_hit, COLLISION_MODE_PROJECTILES);
}

static void shot_projectile(ENEMY2 *enemy) {
  enemy->shot_count++;

  create_projectile(enemy);
}

void create_enemy2() {
  ENEMY2 *enemy = (ENEMY2 *)malloc(sizeof(ENEMY2));

  if (enemy == NULL) {
    fprintf(stderr, "Cannot allocate memory for enemy2");
    exit(1);
  }

  int width = enemy2_sprite.width[0];

  if (last_direction > 0) {
    enemy->base.x = -width;
  } else {
    enemy->base.x = SCREEN_WIDTH + width;
  }

  enemy->sprite_num = 0;
  enemy->base.y = 2 + (rand() % 30);
  enemy->base.width = width;
  enemy->base.height = enemy2_sprite.height[enemy->sprite_num];
  enemy->base.hit_box_x1 = 2;
  enemy->base.hit_box_y1 = 2;
  enemy->base.hit_box_x2 = enemy->base.width - 2;
  enemy->base.hit_box_y2 = enemy->base.height - 2;
  enemy->speed = 6 * last_direction;
  enemy->energy = 1;
  enemy->position = 0;
  enemy->velocity = 0.01;
  enemy->shot_count = 0;

  last_direction = last_direction > 0 ? -1 : 1;

  list_add(enemies_list, enemy);
  add_object_to_collision_list((COL_OBJECT *)enemy, &on_hit, COLLISION_MODE_ALL);

  last_enemy_clock = game_clock_ms;
}

static void create_explosion(ENEMY2 *enemy) {
  EXPLOSION *explosion = malloc(sizeof(EXPLOSION));

  if (explosion == NULL) {
    fprintf(stderr, "Cannot allocate memory for explosion");
    exit(1);
  }

  explosion->x = enemy->base.x + (enemy2_sprite.width[0] - enemy_expl_sprite.width[0]) / 2;
  explosion->y = enemy->base.y;
  explosion->width = enemy_expl_sprite.width[0];
  explosion->height = enemy_expl_sprite.height[0];
  explosion->sprite_num = 0;
  explosion->last_frame_clock = game_clock_ms;

  list_add(explosions, explosion);
}

void start_event__enemy2(LEVEL_EVENT *event) {
  level_event = event;
  start_event_clock = game_clock_ms;
}

static void draw_explosions() {
  for(int i = 0; i < explosions->size; i++) {
    EXPLOSION *explosion = list_get(explosions, i);
    if (explosion->sprite_num >= enemy_expl_sprite.max_sprites || explosion->y > SCREEN_HEIGHT) {
      list_remove(explosions, i);
      continue;
    }

    draw_sprite(enemy_expl_sprite, explosion->sprite_num, explosion->x, explosion->y, IMAGE_DRAW_MODE_NORMAL);

    if(game_clock_ms - explosion->last_frame_clock > 50) {
      explosion->sprite_num++;
      explosion->last_frame_clock = game_clock_ms;
    }

    explosion->y += 0.5;
  }
}

// static void set_projectile_speed(PROJECTILE *projectile) {

static void draw_projectiles() {
  for(int i = 0; i < projectiles->size; i++) {
    PROJECTILE *projectile = list_get(projectiles, i);

    if (projectile->base.y > SCREEN_HEIGHT || projectile->base.x > SCREEN_WIDTH) {
      list_remove(projectiles, i);
      continue;
    }

    draw_sprite(en2_prj, projectile->sprite_num, projectile->base.x, projectile->base.y, IMAGE_DRAW_MODE_NORMAL);

    if(game_clock_ms - projectile->last_frame_clock > 50) {
      projectile->sprite_num++;
      projectile->last_frame_clock = game_clock_ms;

      if(projectile->sprite_num + 1 > en2_prj.max_sprites) {
        projectile->sprite_num = 0;
      }
    }

    projectile->base.y += 0.5;
    projectile->base.x += 0.5;
  }
}

static void enemy_movement(ENEMY2 *enemy) {
  float speed = (float)(enemy->speed * delta_frame_time) / (float)TICKS_PER_SECOND;

  enemy->position += abs(speed);
  enemy->base.x += speed;
  enemy->base.y += sin(enemy->position / SCREEN_WIDTH);
  enemy->speed += enemy->speed * enemy->velocity;

  if (enemy->position > 100 && enemy->shot_count == 1) {
    shot_projectile(enemy);
  }

  if (enemy->position > 20 && enemy->shot_count == 0) {
    shot_projectile(enemy);
  }
}

void draw_event__enemy2() {
  if(enemies_list->size < MAX_ENEMIES && game_clock_ms - start_event_clock <= EVENT_PERIOD && game_clock_ms - last_enemy_clock > 1000) {
    create_enemy2();
  }

  for(int i = 0; i < enemies_list->size; i++) {
    ENEMY2 *enemy = list_get(enemies_list, i);

    if((enemy->speed > 0 && enemy->base.x > SCREEN_WIDTH) || (enemy->speed < 0 && enemy->base.x < -enemy2_sprite.width[0])) {
      remove_object_from_collision_list((COL_OBJECT *)enemy);
      list_remove(enemies_list, i);
      continue;
    }

    if(enemy->energy <= 0) {
      create_explosion(enemy);
      remove_object_from_collision_list((COL_OBJECT *)enemy);
      list_remove(enemies_list, i);
      play_sound(sound_enemy_expl, 64);
      continue;
    }
  }


  // draw enemies
  for(int i = 0; i < enemies_list->size; i++) {
    ENEMY2 *enemy = list_get(enemies_list, i);

    enemy_movement(enemy);

    draw_sprite(
      enemy2_sprite,
      enemy->sprite_num,
      enemy->base.x,
      enemy->base.y,
      enemy->speed > 0 ? IMAGE_DRAW_MODE_NORMAL : IMAGE_DRAW_MODE_FLIP_X
    );

    if (enemy->just_hit) {
      draw_sprite(
        small_expl_sprite,
        0,
        enemy->base.x + ((enemy2_sprite.width[enemy->sprite_num] - small_expl_sprite.width[0]) / 2) + 2,
        enemy->base.y + (enemy2_sprite.height[enemy->sprite_num] / 2) - 3,
        IMAGE_DRAW_MODE_NORMAL
      );

      if(game_clock_ms - enemy->last_hit_clock > 100) {
        enemy->just_hit = 0;
      }
    }
  }

  // // draw explosions
  draw_explosions();

  draw_projectiles();

  // stop event if there are no enemies and enough time has passed
  if(game_clock_ms - start_event_clock > EVENT_PERIOD && enemies_list->size == 0) {
    stop_event(level_event);
  }
}

void clear_event__enemy2() {
  for(int i = 0; i < enemies_list->size; i++) {
    free((ENEMY2 *)list_get(enemies_list, i));
  }

  for(int i = 0; i < explosions->size; i++) {
    free((EXPLOSION *)list_get(explosions, i));
  }

  for(int i = 0; i < projectiles->size; i++) {
    free((PROJECTILE *)list_get(projectiles, i));
  }

  list_free(projectiles);
  list_free(explosions);
  list_free(enemies_list);
}

