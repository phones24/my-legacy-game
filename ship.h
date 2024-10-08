#pragma once

typedef struct {
  int x;
  int y;
  int x_speed;
  int y_speed;
  unsigned long movement_clock;
  unsigned long after_movement_clock;
  int sprite_num;
  int exploding;
  int visible;
  int lives;
  int invincible;
} SHIP;

extern volatile SHIP ship;

void init_ship();
void init_projectile();
void draw_ship_projectile();
void draw_ship();


