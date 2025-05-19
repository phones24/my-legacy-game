#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "all_sprites.h"
#include "collision.h"
#include "graphics.h"
#include "graphics_def.h"
#include "hud.h"
#include "keyboard.h"
#include "level.h"
#include "palette.h"
#include "ship.h"
#include "sound.h"
#include "stars.h"
#include "timer.h"
#include "titlescr.h"

volatile unsigned long begin_frame_ms, delta_frame_ms;
int frames_count = 0;
char fps[10];
int game_stage = 0; // 0 - intro, 1 - game, 2 - end

void draw_fps() {
  if (frames_count == 0) {
    begin_frame_ms = game_clock_ms;
  }

  if (game_clock_ms - begin_frame_ms > 1000) {
    sprintf(fps, "fps: %i", frames_count);
    frames_count = 0;
  } else {
    frames_count++;
  }

  draw_string3x5(SCREEN_WIDTH - 30, 4, fps, 3);
}

void pause() {
  if (keys.p) {
    on_pause = 1;
  }

  while (on_pause) {
    wait_for_vsync();

    if (keys.p) {
      on_pause = 0;
      break;
    }
  }
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
  // sprintf(str, "clock: %lu", frame_count);
  // draw_string(2, 0, str, 3);
  sprintf(str, "clock_ms: %lu", game_clock_ms);
  draw_string(2, 8, str, 3);
  sprintf(str, "delta_frame_time: %lu", delta_frame_time);
  draw_string(2, 16, str, 3);
  // sprintf(str, "a: %d", game_clock_ms % 1000);
  // draw_string(2, 24, str, 3);
}

void exit_handler() {
  restore_keyboard();
  // restore_timer();
}

int main() {
  srand(time(NULL));

  init_keyboard();
  // init_timer();

  atexit(exit_handler);

  init_sound();
  init_collision_list();
  load_sprites();
  init_stars();
  init_ship();
  init_level();
  init_title_screen();

  set_mode_13h_modex();
  init_palette();

  while (1) {
    begin_frame_time = game_clock_ms;
    set_active_page();

    if (game_stage == 0) {
      clear_modex();
      draw_title_screen();
    }

    if (game_stage == 1) {
      clear_modex();

      draw_stars();
      draw_level();
      draw_ship();
      draw_ship_projectile();

      // draw_debug_info();
      pause();

      draw_hud();
    }

    if (game_stage == 0 && keys.enter) {
      // exit_title_screen();
      game_stage = 1;
    }

    if (keys.escape) {
      break;
    }

    draw_fps();
    set_visible_page();
    wait_for_vsync();

    delta_frame_time = game_clock_ms - begin_frame_time;
  }

  getch();

  set_mode_03h();

  // free(image.data);
  // free(image2.data);
  clear_title_screen();
  clear_stars();
  free_sprites();
  clear_level_data();
  clear_collision_list();
  clear_palette();
  clear_sound();

  return 0;
}
