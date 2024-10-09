#pragma once

#define SCREEN_SIZE_HALF 32000
#define SCREEN_SIZE_DIV_4 16000

typedef enum {
  IMAGE_DRAW_MODE_NORMAL,
  IMAGE_DRAW_MODE_FLIP_X,
  IMAGE_DRAW_MODE_FLIP_Y,
} IMAGE_DRAW_MODE;

void init_double_buffer();
void free_double_buffer();

void set_mode_13h();
void set_mode_13h_modex();
void set_mode_03h();

void wait_for_vsync();
void show_double_buffer();

void put_pixel(int x, int y, unsigned char color);
void put_pixel_modex(int x, int y, unsigned char color);

void set_visible_page();
void set_active_page();

void clear_double_buffer();
void clear_modex();
void draw_line(int x0, int y0, int x1, int y1, char color);
void draw_char(int x, int y, char ch, char color);
void draw_string(int x, int y, const char *str, char color);
void draw_image(char * data, int width, int height, int pos_x, int pos_y, IMAGE_DRAW_MODE mode);
