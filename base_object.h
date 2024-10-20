#pragma once

typedef enum {
  COLLISION_MODE_ALL,
  COLLISION_MODE_PROJECTILES,
} COLLISION_MODE;

typedef void (*ON_HIT_FN)(void *object);

typedef struct {
  int collision_id;
  float x;
  float y;
  int width;
  int height;
  int hit_box_x1, hit_box_y1, hit_box_x2, hit_box_y2;
  ON_HIT_FN on_hit;
  COLLISION_MODE collision_mode;
} BASE_OBJECT;
