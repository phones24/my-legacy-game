#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>
#include <conio.h>
#include <math.h>
#include <malloc.h>
#include <time.h>

#include "timer.h"
#include "graphics_def.h"
// #include "font.h"
// #include "sprite_sheet.h"
#include "keyboard.h"
#include "graphics.h"
#include "res.h"
#include "palette.h"
#include "all_sprites.h"
#include "level.h"
#include "stars.h"
#include "ship.h"

volatile unsigned long begin_frame_ms, delta_frame_ms;
volatile unsigned int frames_count = 0;
char fps[10];

void draw_fps()
{
  if (frames_count == 0) {
    begin_frame_ms = game_clock_ms;
  }

  if (game_clock_ms - begin_frame_ms > 1000)
  {
    sprintf(fps, "fps: %d", frames_count);
    frames_count = 0;
  } else {
    frames_count++;
  }

  draw_string(SCREEN_WIDTH - 80, 4, fps, 3);
}

void draw_debug_info() {
  char str[100];

  // sprintf(str, "up: %d", keys.up);
  // draw_string(2, 0, str, 3);
  // sprintf(str, "down: %d", keys.down);
  // draw_string(2, 8, str, 3);
  // sprintf(str, "left: %d", keys.left);
  // draw_string(2, 16, str, 3);
  // sprintf(str, "right: %d", keys.right);
  // draw_string(2, 24, str, 3);
  // sprintf(str, "space: %d", keys.space);
  // draw_string(2, 32, str, 3);
  sprintf(str, "clock: %lu", game_clock);
  draw_string(2, 0, str, 3);
  sprintf(str, "clock_ms: %lu", game_clock_ms);
  draw_string(2, 8, str, 3);
  sprintf(str, "delta_frame_time: %lu", delta_frame_time);
  draw_string(2, 16, str, 3);
  // sprintf(str, "a: %d", game_clock_ms % 1000);
  // draw_string(2, 24, str, 3);

}

void exit_handler() {
  restore_keyboard();
  restore_timer();
}

int main()
{
  // printf("sizeof(unsigned short): %u\n", sizeof(unsigned short));
  // printf("sizeof(unsigned int): %u\n", sizeof(unsigned int));
  // printf("sizeof(unsigned long): %u\n", sizeof(unsigned long));
  // printf("sizeof(char): %u\n", sizeof(char));
  // printf("sizeof(int): %u\n", sizeof(int));
  // printf("sizeof(long): %u\n", sizeof(long));
  // printf("sizeof(float): %u\n", sizeof(float));
  // exit(1);

  srand(time(NULL));

  // IMAGE_RLE image = read_pcx("res\\enemy1.pcx");
  IMAGE image2 = read_bmp("res\\enemy1.bmp");
  LEVEL_DATA level_data = load_level_data("res\\level.txt");

  // printf("event count: %u\n", level_data.count);

  // for (int i = 0; i < level_data.count; i++) {
  //   LEVEL_EVENT *event = &level_data.events[i];
  //   printf("event: %d, clock: %lu\n", event->type, event->clock);
  //   if(event->type == EVENT_PLANET) {
  //     LEVEL_EVENT__PLANET *planet = (LEVEL_EVENT__PLANET *)event->event_data;
  //     printf("\tplanet: %d, %d\n", planet->sprite_num, planet->x);
  //   }
  // }


  init_keyboard();
  init_timer();

  atexit(exit_handler);

  load_sprites();
  init_stars();
  init_ship();
  init_projectile();
  init_level(level_data);

  set_mode_13h_modex();
  init_palette();

  // int y = 0
  while (1) {
    begin_frame_time = game_clock;
    // wait_for_vsync();

    set_active_page();
    clear_modex();

    // draw_image_rle(image.data, image.data_size, image.width, image.height, 20, 100);
    draw_image(image2.data, image2.width, image2.height, 20, 20);
    // for(int i = 0; i <= 200; i+=10) {
    // draw_line(0, i, 200, i, 200);
    // }
    // draw_line(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, rand() % 255);

    // unsigned char c = 0;
    // for (int i = 0; i < SCREEN_WIDTH; i++)
    // {
    //   for (int y = 0; y < SCREEN_HEIGHT; y++)
    //   {
    //     put_pixel(i, y, c);
    //   }

    // draw_line(130, 100, 140, 100, 100);
    //   c++;
    // }
    draw_stars();
    draw_level(level_data);
    draw_ship();
    draw_ship_projectile();
    // draw_debug_info();

    draw_fps();

    set_visible_page();
    wait_for_vsync();

    if(keys.escape) {
      break;
    }

    delta_frame_time = game_clock - begin_frame_time;
  }

  getch();

  set_mode_03h();

  // free(image.data);
  free(image2.data);

  clear_stars();
  free_sprites();
  clear_level_data(level_data);
  clear_palette();

  return 0;
}
