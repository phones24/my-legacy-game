#pragma once

#define SPRITE_FRAMES_NUM 20

typedef struct {
  int width[SPRITE_FRAMES_NUM];
  int height[SPRITE_FRAMES_NUM];
  char *data[SPRITE_FRAMES_NUM];
  int max_sprites;
} SPRITE3;

SPRITE3 load_sprite(const char *name);
void clear_sprite(SPRITE3 sprite);
void draw_sprite(SPRITE3 sprite, int num, int pos_x, int pos_y);


