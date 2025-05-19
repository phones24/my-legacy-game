#include <stdio.h>
#include <stdlib.h>

#include "graphics_def.h"
#include "graphics.h"
#include "timer.h"

#define STARS_COUNT 60

char star_colors[] = {2, 8, 4, 3, 10, 12, 13, 18, 19};
char star_anim[] = {1, 2, 8, 4, 3, 3, 4, 8, 2, 1};

typedef struct {
  int x;
  float y;
  int speed;
  char color;
  char size;
  char is_anim;
  char anim_frame;
} STAR;

STAR stars[STARS_COUNT];
int anim_speed = 2000 / sizeof(star_anim);

void reset_star(STAR *star, int zero_y) {
  star->x = 5 + (rand() % (SCREEN_WIDTH - 5));
  star->y = zero_y == 1 ? 0 : (rand() % 200);
  star->speed = 1 + rand() % 4;
  star->color = star_colors[rand() % sizeof(star_colors)];
  star->is_anim = rand() % 4 == 0 ? 1 : 0;
  star->anim_frame = 0;
}

void init_stars() {
  for(int i = 0; i < STARS_COUNT; i++) {
    reset_star(&stars[i], 0);
  }
}

void draw_star(STAR *star) {
  if(!star->is_anim) {
    put_pixel_modex(star->x, star->y, star->color);

    return;
  }

  if(game_clock_ms % anim_speed == 0) {
    star->anim_frame++;
  }

  if (star->anim_frame >= sizeof(star_anim)) {
    star->anim_frame = 0;
  }

  put_pixel_modex(star->x, star->y, star_anim[star->anim_frame]);
}

void draw_stars() {
  for(int i = 0; i < STARS_COUNT; i++) {
    // float speed = (stars[i].speed * (float)delta_frame_time) / (float)TICKS_PER_SECOND;
    float speed = (stars[i].speed * (float)delta_frame_time) / (float)100;

    if (speed < 0) {
      speed = 0.5;
    }

    if(stars[i].y >= SCREEN_HEIGHT) {
      reset_star(&stars[i], 1);
    }

    if (stars[i].y >= 0) {
      draw_star(&stars[i]);
    }

    stars[i].y += speed;
  }
}

void clear_stars() {
}


