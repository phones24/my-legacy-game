#pragma once

typedef struct
{
  int up;
  int down;
  int left;
  int right;
  int escape;
  int space;
} KEYS;

extern volatile KEYS keys;

void init_keyboard();
void restore_keyboard();

