#pragma once

#include "graphics_def.h"
#include "sprite_sheet.h"

void strip_newline(char *str);
IMAGE read_bmp(const char *filename);
IMAGE_RLE read_pcx(const char* filename);
SPRITE_SHEET read_sprite_sheet(const char *filename);
void clear_image(IMAGE *image);
