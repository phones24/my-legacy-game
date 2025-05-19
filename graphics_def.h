#pragma once

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
// #define SCREEN_SIZE 76800
#define SCREEN_SIZE 64000
#define SCREEN_SIZE_DWORDS (SCREEN_SIZE / 4)

#define TRANSPARENT_COLOR 255

typedef struct {
  unsigned short width;
  unsigned short height;
  char *data;
  char *data_planar;
} IMAGE;

typedef struct {
  unsigned short width;
  unsigned short height;
  char *data;
  long data_size;
} IMAGE_RLE;
