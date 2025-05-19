#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "math.h"
#include "midasdll.h"
#include "res.h"
#include "timer.h"

#define STARS_COUNT 100

typedef struct {
  float x;
  float y;
  float speed_x;
  float speed_y;
  char color;
  char size;
  char is_anim;
  char anim_frame;
} STAR;

static IMAGE title_screen;
static MIDASmodule module;
static MIDASmodulePlayHandle playHandle;
static STAR stars[STARS_COUNT];
static char star_colors[] = {2, 8, 4, 3, 10, 12, 13, 18, 19};
static char star_anim[] = {1, 2, 8, 4, 3, 3, 4, 8, 2, 1};
static int anim_speed = 2000 / sizeof(star_anim);

float generate_random_float(float min, float max) {
  return min + ((float)rand() / RAND_MAX) * (max - min);
}

static void reset_star(STAR *star, int zero_y) {
  float angle = generate_random_float(-0.5, 2);
  float rad = zero_y ? 210 : (-210 + rand() % (210 * 2));
  float speed = generate_random_float(0.5, 4);

  star->x = 160 + rad * cos(angle);
  star->y = 100 + rad * sin(angle);
  star->speed_x = -speed * 1.6;
  star->speed_y = -speed * 1.1;
  star->color = star_colors[rand() % sizeof(star_colors)];
  star->is_anim = rand() % 4 == 0 ? 1 : 0;
  star->anim_frame = 0;
}

static void init_stars() {
  for (int i = 0; i < STARS_COUNT; i++) {
    reset_star(&stars[i], 0);
  }
}

void init_title_screen() {
  title_screen = read_bmp("res\\title.bmp");

  if ((module = MIDASloadModule("res\\intro.xm")) == 0) {
    midas_error();
  }

  if((playHandle = MIDASplayModule(module, TRUE)) == 0) {
    midas_error();
  }

  init_stars();
}

static void draw_star(STAR *star) {
  if (star->y <= 0 || star->x <= 0 || star->x >= SCREEN_WIDTH ||
      star->y >= SCREEN_HEIGHT) {
    return;
  }

  char col = get_pixel_modex(star->x, star->y);

  if (!star->is_anim && col == 0) {
    put_pixel_modex(star->x, star->y, star->color);

    return;
  }

  if (game_clock_ms % anim_speed == 0) {
    star->anim_frame++;
  }

  if (star->anim_frame >= sizeof(star_anim)) {
    star->anim_frame = 0;
  }

  if (col == 0) {
    put_pixel_modex(star->x, star->y, star_anim[star->anim_frame]);
  }
}

static void draw_stars() {
  for (int i = 0; i < STARS_COUNT; i++) {
    float speed_x = (stars[i].speed_x * (float)delta_frame_time) / 100.0;
    float speed_y = (stars[i].speed_y * (float)delta_frame_time) / 100.0;

    if (stars[i].y <= 0 || stars[i].x <= 0) {
      reset_star(&stars[i], 1);
    }

    draw_star(&stars[i]);

    stars[i].x += stars[i].speed_x;
    stars[i].y += stars[i].speed_y;
  }
}

void draw_title_screen() {
  draw_image_full_screen(title_screen);

  draw_stars();

  char str[100];
  sprintf(str, "Press ENTER to start");
  draw_string3x5(227, 84, str, 5);
  sprintf(str, "Press ESC to exit");
  draw_string3x5(239, 92, str, 5);
  sprintf(str, "phones24, 2024");
  draw_string3x5(5, 190, str, 8);
}

void clear_title_screen() {
  clear_image(&title_screen);
  MIDASfreeModule(module);
}
