#ifndef STARS_H
#define STARS_H

#define STARS_COUNT 30
#define PLANETS_COUNT 3

char star_colors[] = {2, 8, 10, 11, 12, 13, 18, 19};

typedef struct {
  int x;
  int y;
  int speed;
  char color;
  char size;
} STAR;

STAR stars[STARS_COUNT];

void reset_star(int i, int zero_y) {
  stars[i].x = 5 + (rand() % (SCREEN_WIDTH - 5));
  stars[i].y = zero_y == 1 ? 0 : (rand() % 200);
  stars[i].speed = 5 + rand() % 15;
  stars[i].color = rand() % sizeof(star_colors);
}

void init_stars() {
  for(int i = 0; i < STARS_COUNT; i++) {
    reset_star(i, 0);
  }
}

void draw_star(STAR *star) {
  put_pixel_modex(star->x, star->y, star->color);
}

void draw_stars() {
  for(int i = 0; i < STARS_COUNT; i++) {
    int speed = (stars[i].speed * delta_frame_time) / TICKS_PER_SECOND;

    if(stars[i].y >= SCREEN_HEIGHT) {
      reset_star(i, 1);
    }

    draw_star(&stars[i]);

    stars[i].y += speed;
  }
}

void clear_stars() {
}


#endif
