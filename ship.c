#include "keyboard.h"
#include "all_sprites.h"
#include "graphics_def.h"
#include "collision.h"
#include "timer.h"
#include "sprite.h"
#include "event_enemy1.h"
#include "ship.h"

#define PROJECTILES_NUM 20

typedef struct {
  int x;
  int y;
  int visible;
} PROJECTILE;

volatile struct {
  int x;
  int y;
  int x_speed;
  int y_speed;
  unsigned long movement_clock;
  unsigned long after_movement_clock;
  int sprite_num;
} ship;

PROJECTILE projectile[PROJECTILES_NUM];
int x_movement = 0;
int last_x_direction = 0;
int projectile_x_offset = 0;
int projectile_speed = 20;
int projectile_width = 0;
int projectile_height = 0;
int ship_speed = 20;

void init_ship() {
  ship.x = 140;
  ship.y = 100;
  ship.x_speed = 0;
  ship.y_speed = 0;
  ship.movement_clock = 0;
  ship.sprite_num = 2;
}

void init_projectile() {
  for (int i = 0; i < PROJECTILES_NUM; i++) {
    projectile[i].x = 0;
    projectile[i].y = 0;
    projectile[i].visible = 0;
  }

  projectile_width = ship_prj_sprite.width[0];
  projectile_height = ship_prj_sprite.height[0];
  projectile_x_offset = ship_sprite.width[ship.sprite_num] / 2 - projectile_width / 2 - 1;
}

void shot_projectile() {
  static unsigned long last_shot_clock = 0;

  if (keys.space) {
    if (game_clock_ms - last_shot_clock < 100) {
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

      break;
    }
  }

  if (!keys.space) {
    last_shot_clock = 0;
  }
}

int check_projectile_collision(int x, int y, int width, int height) {
  for(int i = 0; i < collision_objects_count; i++) {
    COL_OBJECT *object = collision_objects[i];

    if (x + width > object->base.x && x < object->base.x + object->base.width && y + height > object->base.y && y < object->base.y + object->base.height) {
      object->base.hit(object);
      return 1;
    }
  }

  return 0;
}

void draw_ship_projectile() {
  shot_projectile();

  int speed = (projectile_speed * delta_frame_time) / TICKS_PER_SECOND;
  static int projectile_sprite_num = 0;

  if(game_clock_ms % 20 == 0) {
    projectile_sprite_num = projectile_sprite_num == 0 ? 1 : 0;
  }

  for (int i = 0; i < PROJECTILES_NUM; i++) {
    PROJECTILE *prj = &projectile[i];

    if (prj->visible == 0) {
      continue;
    }

    if (prj->y <= -projectile_height) {
      prj->visible = 0;
      continue;
    }

    if(check_projectile_collision(prj->x, prj->y, projectile_width, projectile_height)) {
      prj->visible = 0;
      continue;
    }

    draw_sprite(ship_prj_sprite, projectile_sprite_num, prj->x, prj->y);

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

void draw_ship() {
  int x_speed_mult = keys.left ? -1 : 1;
  int y_speed_mult = keys.up ? -1 : 1;

  if (keys.right || keys.left || keys.up || keys.down) {
    int speed = (ship_speed * delta_frame_time) / TICKS_PER_SECOND;

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

  // svae if ship is moving horizontally
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

  // draw ship
  draw_sprite(ship_sprite, ship.sprite_num, ship.x, ship.y);
}


