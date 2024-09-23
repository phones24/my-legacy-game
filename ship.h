#ifndef SHIP_H
#define SHIP_H

#define PROJECTILES_NUM 20

typedef struct {
  int x;
  int y;
  int y_speed;
  int visible;
} PROJECTILE;

struct {
  int x;
  int y;
  int speed;
  int x_speed;
  int y_speed;
  unsigned long movement_clock;
  unsigned long after_movement_clock;
  int sprite_num;
} ship;

SPRITE3 ship_sprite;
SPRITE3 prj_sprite;
PROJECTILE projectile[PROJECTILES_NUM];
int projectile_x_offset = 0;

void init_ship() {
  ship_sprite = load_sprite("ship3");

  ship.x = 140;
  ship.y = 100;
  ship.speed = 4;
  ship.x_speed = 0;
  ship.y_speed = 0;
  ship.movement_clock = 0;
  ship.sprite_num = 2;
}

void init_projectile() {
  prj_sprite = load_sprite("ship_prj");

  for (int i = 0; i < PROJECTILES_NUM; i++) {
    projectile[i].x = 0;
    projectile[i].y = 0;
    projectile[i].y_speed = 9;
    projectile[i].visible = 0;
  }

  projectile_x_offset = ship_sprite.width[ship.sprite_num] / 2 - prj_sprite.width[0] / 2 - 1;
}

void shot_projectile() {
  static unsigned long last_shot_clock = 0;

  if (keys.space) {
    if (game_clock - last_shot_clock < 10) {
      return;
    }

    for(int i = 0; i < PROJECTILES_NUM; i++) {
      PROJECTILE *prj = &projectile[i];

      if (prj->visible == 1) {
        continue;
      }

      prj->x = ship.x + projectile_x_offset;
      prj->y = ship.y - prj_sprite.height[0];
      prj->visible = 1;
      last_shot_clock = game_clock;

      break;
    }
  }

  if (!keys.space) {
    last_shot_clock = 0;
  }
}

void draw_ship_projectile() {
  shot_projectile();

  static int projectile_sprite_num = 0;

  if(game_clock % 30) {
    projectile_sprite_num = projectile_sprite_num == 0 ? 1 : 0;
  }

  for (int i = 0; i < PROJECTILES_NUM; i++) {
    PROJECTILE *prj = &projectile[i];

    if (prj->visible == 0) {
      continue;
    }

    if (prj->y <= 0) {
      prj->visible = 0;
      continue;
    }

    draw_sprite(prj_sprite, projectile_sprite_num, prj->x, prj->y);

    prj->y -= prj->y_speed;
  }
}

void draw_ship() {
    int x_speed_mult = keys.left ? -1 : 1;
    int y_speed_mult = keys.up ? -1 : 1;

    if (keys.right || keys.left || keys.up || keys.down) {
      if (ship.movement_clock > 0) {
        unsigned long delta = game_clock - ship.movement_clock;

        if(delta > 10) {
          if (keys.right) {
            ship.sprite_num = 3;
          }

          if (keys.left ) {
            ship.sprite_num = 1;
          }
        }

        if(delta > 40) {
          if (keys.right) {
            ship.sprite_num = 4;
          }

          if (keys.left ) {
            ship.sprite_num = 0;
          }
        }
      }

      ship.x_speed = ship.speed * x_speed_mult;
      ship.y_speed = ship.speed * y_speed_mult;
    }

    // stop movement if no keys are pressed
    if(!keys.left && !keys.right) {
      ship.x_speed = 0;
    }

    if(!keys.up && !keys.down) {
      ship.y_speed = 0;
    }

    // reset movement clock if no keys are pressed
    if(!keys.up && !keys.down && !keys.left && !keys.right) {
      ship.movement_clock = 0;
      ship.sprite_num = 2;
    }

    // move ship
    ship.x += ship.x_speed;
    ship.y += ship.y_speed;

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
        ship.movement_clock = game_clock;
      }
    }

    // draw ship
    draw_sprite(ship_sprite, ship.sprite_num, ship.x, ship.y);
}

void clear_ship() {
  clear_sprite(ship_sprite);
}

void clear_projectile() {
  clear_sprite(prj_sprite);
}

#endif

