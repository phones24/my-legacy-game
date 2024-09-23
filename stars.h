#ifndef STARS_H
#define STARS_H

#define STARS_COUNT 30
#define PLANETS_COUNT 3

char star_colors[] = {2, 9, 10, 11, 12, 13};

struct {
  int x;
  int y;
  int speed;
  char color;
} stars[STARS_COUNT];

void reset_star(int i, int zero_y) {
  stars[i].x = 5 + (rand() % (SCREEN_WIDTH - 5));
  stars[i].y = zero_y ? 0 : rand() % (SCREEN_HEIGHT - 20);
  stars[i].speed = 1 + rand() % 5;
  stars[i].color = rand() % sizeof(star_colors);
}

void init_stars() {
  for(int i = 0; i < STARS_COUNT; i++) {
    reset_star(i, 0);
  }
}

void draw_stars() {
  for(int i = 0; i < STARS_COUNT; i++) {
    if(stars[i].y >= (SCREEN_HEIGHT - stars[i].speed)) {
      reset_star(i, 1);
    }

    stars[i].y += stars[i].speed;

    // put_pixel(stars[i].x, stars[i].y, stars[i].color);
    put_pixel_modex(stars[i].x, stars[i].y, stars[i].color);
  }
}

void clear_stars() {
}


#endif
