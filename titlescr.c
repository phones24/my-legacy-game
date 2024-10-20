#include <stdio.h>
#include <stdlib.h>

#include "res.h"
#include "graphics.h"
#include "midasdll.h"

IMAGE title_screen;
MIDASmodule module;
MIDASmodulePlayHandle playHandle;

void init_title_screen() {
  title_screen = read_bmp("res\\title.bmp");

  if((module = MIDASloadModule("res\\intro.xm")) == 0) {
    midas_error();
  }

  if((playHandle = MIDASplayModule(module, TRUE)) == 0) {
    midas_error();
  }
}

void draw_title_screen() {
  draw_image(title_screen.data, title_screen.width, title_screen.height, 0, 0, IMAGE_DRAW_MODE_NORMAL);

  char str[100];
  sprintf(str, "Press ENTER to start");
  draw_string3x5(227, 84, str, 5);
  sprintf(str, "Press ESC to exit");
  draw_string3x5(239, 92, str, 5);

  sprintf(str, "Yura V, 2024");
  draw_string3x5(5, 190, str, 8);
}

void clear_title_screen() {
  clear_image(&title_screen);
  MIDASfreeModule(module);
}
