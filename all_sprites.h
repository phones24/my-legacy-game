#ifndef SPRITES_LOADER_H
#define SPRITES_LOADER_H

#include "sprite.h"

extern SPRITE3 ship_sprite;
extern SPRITE3 ship_prj_sprite;
extern SPRITE3 planets_sprite;
extern SPRITE3 nebula_sprite;
extern SPRITE3 enemy1_sprite;

void load_sprites();
void free_sprites();

#endif
