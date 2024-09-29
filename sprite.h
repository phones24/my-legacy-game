#pragma once

#define SPRITES_NUM 20

typedef struct {
  int width[SPRITES_NUM];
  int height[SPRITES_NUM];
  char *data[SPRITES_NUM];
  int max_sprites;
} SPRITE3;

SPRITE3 load_sprite(const char *name);
void clear_sprite(SPRITE3 sprite);
void draw_sprite(SPRITE3 sprite, int num, int pos_x, int pos_y);


