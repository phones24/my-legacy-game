#include <stdio.h>

#include "keyboard.h"
#include "all_sprites.h"
#include "graphics_def.h"
#include "collision.h"
#include "timer.h"
#include "sprite.h"
#include "event_enemy1.h"
#include "ship.h"
#include "sound.h"

#define PROJECTILES_NUM 20
#define SHIP_SPEED 20
#define PROJECTILE_SPEED 20

typedef struct {
  int x;
  int y;
  int visible;
} PROJECTILE;

volatile SHIP ship;
static PROJECTILE projectile[PROJECTILES_NUM];
static int x_movement = 0;
static int last_x_direction = 0;
static int projectile_x_offset = 0;
static int projectile_width = 0;
static int projectile_height = 0;
static int projectile_sprite_num = 0;
static int ship_expl_sprite_num = 0;
static unsigned long last_shot_clock = 0;
static unsigned long last_exp_frame_clock = 0;
static unsigned long invincible_clock = 0;
static int invincible_draw = 0;

void set_invincible() {
  ship.invincible = 1;
  invincible_clock = game_clock_ms;
}

void reset_ship() {
  ship.x = 140;
  ship.y = 100;
  ship.x_speed = 0;
  ship.y_speed = 0;
  ship.movement_clock = 0;
  ship.sprite_num = 2;
  ship.exploding = 0;
  ship.visible = 1;
}

void init_ship() {
  reset_ship();

  ship.lives = 3;

  set_invincible();
  init_projectile();
}

void revive_ship() {
  reset_ship();
  set_invincible();
  init_projectile();

  ship.lives--;

  last_shot_clock = 0;
  last_exp_frame_clock = 0;
  ship_expl_sprite_num = 0;
}

void init_projectile() {
  for (int i = 0; i < PROJECTILES_NUM; i++) {
    projectile[i].x = 0;
    projectile[i].y = 0;
    projectile[i].visible = 0;
  }

  projectile_width = ship_prj_sprite.width[0];
  projectile_height = ship_prj_sprite.height[0];
  projectile_x_offset = (ship_sprite.width[ship.sprite_num] - projectile_width) / 2;
}

void shot_projectile() {
  if (keys.space) {
    if (game_clock_ms - last_shot_clock < 150) {
      return;
    }

    for(int i = 0; i < PROJECTILES_NUM; i++) {
      PROJECTILE *prj = &projectile[i];

      if (prj->visible == 1) {
        continue;
      }

      prj->x = ship.x + projectile_x_offset;
      prj->y = ship.y - projectile_height;

      prj->visible = 1;
      last_shot_clock = game_clock_ms;

      play_sound(sound_ship_shot, 30);

      break;
    }
  }

  if (!keys.space) {
    last_shot_clock = 0;
  }
}

int check_projectile_collision(PROJECTILE *prj) {
  int x = prj->x,
    y = prj->y,
    width = projectile_width,
    height = projectile_height;

  for(int i = 0; i < collision_objects->size; i++) {
    COL_OBJECT *object = list_get(collision_objects, i);

    if (x + width > object->base.x + object->base.hit_box_x1 &&
        x < object->base.x + object->base.hit_box_x2 &&
        y + height - 10 > object->base.y + object->base.hit_box_y1 && // 10 is aprox height of ship engine exhaust
        y < object->base.y + object->base.hit_box_y2 &&
        object->base.collision_mode == COLLISION_MODE_ALL
    ) {
      object->base.on_hit(object);
      return 1;
    }
  }

  return 0;
}

int check_ship_collision() {
  int x = ship.x,
    y = ship.y,
    width = ship_sprite.width[ship.sprite_num],
    height = ship_sprite.height[ship.sprite_num];

  for(int i = 0; i < collision_objects->size; i++) {
    COL_OBJECT *object = list_get(collision_objects, i);

    if (x + width > object->base.x && x < object->base.x + object->base.width && y + height > object->base.y && y < object->base.y + object->base.height) {
      object->base.on_hit(object);
      return 1;
    }
  }

  return 0;
}

void draw_ship_projectile() {
  if(ship.exploding || !ship.visible) {
    return;
  }

  shot_projectile();

  int speed = (PROJECTILE_SPEED * delta_frame_time) / TICKS_PER_SECOND;

  for (int i = 0; i < PROJECTILES_NUM; i++) {
    PROJECTILE *prj = &projectile[i];

    if (prj->visible == 0) {
      continue;
    }

    if (prj->y <= -projectile_height) {
      prj->visible = 0;
      continue;
    }

    // if(check_projectile_collision(prj->x, prj->y, projectile_width, projectile_height)) {
    if(check_projectile_collision(prj)) {
      prj->visible = 0;
      continue;
    }

    draw_sprite(ship_prj_sprite, projectile_sprite_num, prj->x, prj->y, IMAGE_DRAW_MODE_NORMAL);

    prj->y -= speed;
  }
}

void post_movement_anim_1() {
  if(keys.left || keys.right) {
    return;
  }

  ship.sprite_num = last_x_direction == 1 ? 3 : 1;
}

void post_movement_anim_2() {
  if(keys.left || keys.right) {
    return;
  }

  ship.sprite_num = 2;
}

void check_invincibility() {
  if(!ship.invincible) {
    return;
  }

  static unsigned long last_invincible_blink = 0;

  if(game_clock_ms - invincible_clock > 2000) {
    ship.invincible = 0;
  }

  if(game_clock_ms - last_invincible_blink > 20) {
    invincible_draw = !invincible_draw;
    last_invincible_blink = game_clock_ms;
  }
}

void draw_ship_explosion() {
  draw_sprite(
    ship_expl_sprite,
    ship_expl_sprite_num,
    ship.x + (ship_sprite.width[ship.sprite_num] - ship_expl_sprite.width[ship_expl_sprite_num]) / 2,
    ship.y + (ship_sprite.height[ship.sprite_num] - ship_expl_sprite.height[ship_expl_sprite_num]) / 2,
    IMAGE_DRAW_MODE_NORMAL
  );

  if(game_clock_ms - last_exp_frame_clock > 100) {
    ship_expl_sprite_num++;
    last_exp_frame_clock = game_clock_ms;
  }

  if(ship_expl_sprite_num > ship_expl_sprite.max_sprites) {
    ship.visible = 0;
    ship.exploding = 0;

    schedule_task(1000, revive_ship);
  }
}

void draw_ship() {
  if(!ship.visible) {
    return;
  }

  if(ship.exploding) {
    draw_ship_explosion();
    return;
  }

  if(!ship.exploding && !ship.invincible && check_ship_collision()) {
    ship.exploding = 1;
    play_sound(sound_ship_expl, 50);
    return;
  }

  check_invincibility();

  int x_speed_mult = keys.left ? -1 : 1;
  int y_speed_mult = keys.up ? -1 : 1;

  if (keys.right || keys.left || keys.up || keys.down) {
    int speed = (SHIP_SPEED * delta_frame_time) / TICKS_PER_SECOND;

    if (ship.movement_clock > 0) {
      unsigned long delta = game_clock_ms - ship.movement_clock;

      if(delta > 100) {
        if (keys.right) {
          ship.sprite_num = 3;
        }

        if (keys.left ) {
          ship.sprite_num = 1;
        }
      }

      if(delta > 200) {
        if (keys.right) {
          ship.sprite_num = 4;
        }

        if (keys.left ) {
          ship.sprite_num = 0;
        }
      }
    }

    ship.x_speed = speed * x_speed_mult;
    ship.y_speed = speed * y_speed_mult;
    last_x_direction = x_speed_mult;
  }

  // stop movement if no keys are pressed
  if(!keys.left && !keys.right) {
    ship.x_speed = 0;

    // schedule post movement animation
    if (x_movement) {
      schedule_task(100, post_movement_anim_1);
      schedule_task(200, post_movement_anim_2);
    }

    x_movement = 0;
  }

  if(!keys.up && !keys.down) {
    ship.y_speed = 0;
  }

  // reset movement clock if no keys are pressed
  if(!keys.up && !keys.down && !keys.left && !keys.right) {
    ship.movement_clock = 0;
  }

  // move ship
  ship.x += ship.x_speed;
  ship.y += ship.y_speed;

  // save if ship is moving horizontally
  x_movement = ship.x_speed != 0 ? 1 : 0;

  // check screen boundaries
  if(ship.x < 0 ) {
    ship.x = 0;
  }

  if(ship.x >= SCREEN_WIDTH - ship_sprite.width[ship.sprite_num]) {
    ship.x = SCREEN_WIDTH - ship_sprite.width[ship.sprite_num];
  }

  if(ship.y < 0 ) {
    ship.y = 0;
  }

  if(ship.y >= SCREEN_HEIGHT - ship_sprite.height[ship.sprite_num]) {
    ship.y = SCREEN_HEIGHT - ship_sprite.height[ship.sprite_num];
  }

  if(ship.x < 0 || ship.y < 0 || ship.x >= SCREEN_WIDTH || ship.y >= SCREEN_HEIGHT) {
    ship.x = 0;
    ship.y = 0;
  }

  // check for movement
  if(keys.up || keys.down || keys.left || keys.right) {
    if (ship.movement_clock == 0) {
      ship.movement_clock = game_clock_ms;
    }
  }

  if (!invincible_draw && ship.invincible) {
    return;
  }

  // draw ship
  draw_sprite(ship_sprite, ship.sprite_num, ship.x, ship.y, IMAGE_DRAW_MODE_NORMAL);
}


