#include "all_sprites.h"

SPRITE3 ship_sprite;
SPRITE3 ship_prj_sprite;
SPRITE3 ship_expl_sprite;
SPRITE3 planets_sprite;
// SPRITE3 nebula_sprite;
// SPRITE3 enemy1_sprite;
SPRITE3 enemy2_sprite;
SPRITE3 enemy1_expl_sprite;
SPRITE3 small_expl_sprite;
SPRITE3 rock_sprite;

void load_sprites() {
  ship_sprite = load_sprite("ship3");
  ship_prj_sprite = load_sprite("shipprj2");
  ship_expl_sprite = load_sprite("shipexpl");
  planets_sprite = load_sprite("planets");
  // nebula_sprite = load_sprite("nebula");
  // enemy1_sprite = load_sprite("enemy1");
  enemy2_sprite = load_sprite("enemy2");
  enemy1_expl_sprite = load_sprite("en1expl");
  small_expl_sprite = load_sprite("smexpl");
  rock_sprite = load_sprite("rock3");
}

void free_sprites() {
  clear_sprite(ship_sprite);
  clear_sprite(ship_prj_sprite);
  clear_sprite(ship_expl_sprite);
  clear_sprite(planets_sprite);
  // clear_sprite(nebula_sprite);
  // clear_sprite(enemy1_sprite);
  clear_sprite(enemy2_sprite);
  clear_sprite(enemy1_expl_sprite);
  clear_sprite(small_expl_sprite);
  clear_sprite(rock_sprite);
}
