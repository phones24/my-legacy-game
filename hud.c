#include <stdio.h>

#include "ship.h"
#include "graphics.h"


void draw_hud() {
  char str[5];

  sprintf(str, "LIVES: %i", ship.lives);
  draw_string(2, 2, str, 3);
}
