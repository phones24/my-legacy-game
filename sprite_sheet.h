#pragma once

typedef struct {
  unsigned int x0;
  unsigned int y0;
  unsigned int x1;
  unsigned int y1;
} SPRITE_DEF;

typedef struct {
  unsigned int count;
  SPRITE_DEF sprites_def[20];
} SPRITE_SHEET;

