#pragma once

typedef struct {
  int id;
  float x;
  float y;
  int width;
  int height;
  void (*hit)(void *object);
} BASE_OBJECT;

