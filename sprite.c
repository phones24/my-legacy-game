#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

#include "graphics_def.h"
#include "graphics.h"
#include "sprite_sheet.h"
#include "res.h"
#include "sprite.h"

SPRITE3 load_sprite(const char *name)
{
  char bmp_filename[100];
  char txt_filename[100];

  sprintf(bmp_filename, "res\\%s.bmp", name);
  sprintf(txt_filename, "res\\%s.txt", name);

  IMAGE image = read_bmp(bmp_filename);
  SPRITE_SHEET sprite_sheet = read_sprite_sheet(txt_filename);
  SPRITE3 sprite;

  for (int num = 0; num < sprite_sheet.count; num++) {
    SPRITE_DEF sprite_def = sprite_sheet.sprites_def[num];

    sprite.width[num] = sprite_def.x1 - sprite_def.x0 + 1;
    sprite.height[num] = sprite_def.y1 - sprite_def.y0 + 1;
    sprite.data[num] = malloc(sprite.width[num] * sprite.height[num]);

    if(sprite.data[num] == NULL) {
      printf("Error: Could not allocate memory for sprite data: %s, size: %u\n", txt_filename, sprite.width[num] * sprite.height[num]);
      exit(1);
    }

    int index = 0;
    for (int y = sprite_def.y0; y <= sprite_def.y1; y++) {
      for (int x = sprite_def.x0; x <= sprite_def.x1; x++) {
        sprite.data[num][index++] = image.data[x + y * image.width];
      }
    }

  }

  sprite.max_sprites = sprite_sheet.count;

  for (int i = sprite_sheet.count; i < SPRITES_NUM; i++) {
    sprite.data[i] = NULL;
  }

  free(image.data);

  return sprite;
}

void clear_sprite(SPRITE3 sprite)
{
  for (int i = 0; i < SPRITES_NUM; i++) {
    if(sprite.data[i] != NULL) {
      free(sprite.data[i]);
    }
  }
}

void draw_sprite(SPRITE3 sprite, int num, int pos_x, int pos_y) {
  if (sprite.data[num] == NULL) {
    return;
  }

  draw_image(sprite.data[num], sprite.width[num], sprite.height[num], pos_x, pos_y);
}

