#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>

#include "font.h"
#include "font3x5.h"
#include "graphics.h"
#include "graphics_def.h"

char *video_mem = (char *)0xA0000;

static unsigned char page = 0;
static unsigned int video_mem_offset = 0;

void set_mode_03h() {
  union REGS regs;

  regs.h.ah = 0x00;
  regs.h.al = 0x03;

  int386(0x10, &regs, &regs);
}

void set_mode_13h_modex() {
  union REGS regs;

  regs.h.ah = 0x00;
  regs.h.al = 0x13;

  int386(0x10, &regs, &regs);

  outp(0x3c4, 0x04);
  outp(0x3c5, 0x06);

  outp(0x3d4, 0x17);
  outp(0x3d5, 0xE3);

  outp(0x3d4, 0x14);
  outp(0x3d5, 0x00);

  memset(video_mem, 0, 16 * 1024);
}

void wait_for_vsync() {
  while ((inp(0x03DA) & 0x08));
  while (!(inp(0x03DA) & 0x08));
}

void put_pixel_modex(int x, int y, char color) {
  outp(0x3C4, 0x02);
  outp(0x3C5, 1 << (x & 0x3));

  _asm {
    push eax
    push ebx
    push ecx
    push edx
    push edi

    mov eax, y
    mov edx, 80
    mul edx
    mov edx, x
    shr edx, 2
    add eax, edx

    add eax, video_mem_offset

    mov edi, video_mem
    mov edx, eax
    mov al, color
    mov [edi + edx], al

    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax
  }
}

char get_pixel_modex(int x, int y) {
  outp(0x3ce, 0x04);
  outp(0x3cf, x & 0x3);

  return video_mem[video_mem_offset + (SCREEN_WIDTH / 4) * y + (x >> 2)];
}

void set_visible_page() {
  unsigned int start_address = page * SCREEN_SIZE_DIV_4;

  outp(0x3d4, 0x0C);
  outp(0x3d5, (start_address & 0xFF00) >> 8);
  outp(0x3d4, 0x0D);
  outp(0x3d5, start_address & 0x00FF);

  page = page == 0 ? 1 : 0;
}

void set_active_page() { video_mem_offset = page * SCREEN_SIZE_DIV_4; }

void clear_modex() {
  outp(0x3C4, 0x02);
  outp(0x3C5, 0x0F);

  _asm {
    push edi
    push edx
    mov  edi, video_mem
    add  edi, video_mem_offset
    xor  eax, eax
    mov  ecx, SCREEN_SIZE_DIV_4 / 4
    rep  stosd
    pop  edx
    pop  edi
  }
}

void draw_line(int x0, int y0, int x1, int y1, char color) {
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    // put_pixel(x0, y0, color);
    put_pixel_modex(x0, y0, color);

    if (x0 == x1 && y0 == y1) {
      break;
    }

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void draw_char(int x, int y, char ch, char color) {
  int font_index = (char)ch * FONT_HEIGHT;
  for (unsigned int row = 0; row < FONT_HEIGHT; row++) {
    char line = font[font_index + row];
    for (unsigned int col = 0; col < FONT_WIDTH; col++) {
      if (line & (1 << col)) {
        put_pixel_modex(x + col, y + row, color);
      }
    }
  }
}

void draw_string(int x, int y, const char *str, char color) {
  while (*str) {
    if (x + FONT_WIDTH >= SCREEN_WIDTH) {
      x = 0;
      y += FONT_HEIGHT;

      if (y + FONT_HEIGHT > SCREEN_HEIGHT) {
        return;
      }
    }
    draw_char(x, y, *str, color);
    x += FONT_WIDTH;
    str++;
  }
}

void draw_char3x5(int x, int y, char ch, char color) {
  char *glyph = font3x5[ch - ' '];

  for (unsigned int row = 0; row < FONT3x5_HEIGHT; row++) {
    for (unsigned int col = 0; col < FONT3x5_WIDTH; col++) {
      char px = glyph[col + row * FONT3x5_WIDTH];
      if (px == '#') {
        put_pixel_modex(x + col, y + row, color);
      }
    }
  }
}

void draw_string3x5(int x, int y, const char *str, char color) {
  while (*str) {
    if (x + FONT3x5_WIDTH >= SCREEN_WIDTH) {
      x = 0;
      y += FONT3x5_HEIGHT;

      if (y + FONT3x5_HEIGHT > SCREEN_HEIGHT) {
        return;
      }
    }
    draw_char3x5(x, y, *str, color);
    x += FONT3x5_WIDTH + 1;
    str++;
  }
}

void draw_image(char *data, int width, int height, int pos_x, int pos_y,
                IMAGE_DRAW_MODE mode) {
  if (pos_y + height < 0 || pos_y >= SCREEN_HEIGHT) {
    return;
  }

  if (pos_x + width < 0 || pos_x >= SCREEN_WIDTH) {
    return;
  }

  char color;
  int final_x;
  int final_y = pos_y - 1;

  for (int y = 0; y < height; y++) {
    final_y++;

    if (final_y < 0 || final_y >= SCREEN_HEIGHT) {
      continue;
    }

    char *line = &data[y * width];
    for (int x = 0; x < width; x++) {
      color = *line++;
      final_x = mode == IMAGE_DRAW_MODE_FLIP_X ? pos_x + width - x : pos_x + x;

      if (color != TRANSPARENT_COLOR && final_x >= 0 &&
          final_x < SCREEN_WIDTH) {
        put_pixel_modex(final_x, final_y, color);
      }
    }
  }
}

void draw_image_full_screen(IMAGE image) {
  outp(0x3C4, 0x02);

  for (int p = 0; p < 4; p++) {
    outp(0x3C5, 1 << (p & 3));

    char *src = image.data_planar + SCREEN_SIZE_DIV_4 * p;
    char *dest = video_mem + video_mem_offset;
    int count = SCREEN_SIZE_DIV_4;

    _asm {
        mov esi, src
        mov edi, dest
        mov ecx, count

        shr ecx, 2
        rep movsd

        mov ecx, count
        and ecx, 3
        rep movsb
    }
  }
}

void draw_image_rle(const char *data, unsigned long data_size, int width,
                    int height, int pos_x, int pos_y) {
  if (pos_y + height < 0 || pos_y >= SCREEN_HEIGHT) {
    return;
  }

  if (pos_x + width < 0 || pos_x >= SCREEN_WIDTH) {
    return;
  }

  int draw_x = pos_x, draw_y = pos_y;
  long i = 0;

  while (i < data_size && draw_y <= SCREEN_HEIGHT) {
    char value = data[i++];
    int count = 1;

    if ((value & 0xC0) == 0xC0) {
      count = value & 0x3F;
      value = data[i++];
    }

    while (count > 0 && draw_y < SCREEN_HEIGHT) {
      if (draw_x < 0) {
        int skip = (count < -draw_x) ? count : -draw_x;
        draw_x += skip;
        count -= skip;
        continue;
      }

      if (draw_x >= SCREEN_WIDTH) {
        draw_x = pos_x;
        draw_y++;
        continue;
      }

      int remaining_in_row = SCREEN_WIDTH - draw_x;
      int to_draw = (count < remaining_in_row) ? count : remaining_in_row;

      if (draw_x + to_draw >= SCREEN_WIDTH) {
        to_draw = SCREEN_WIDTH - draw_x;
      }

      if (value != TRANSPARENT_COLOR) {
        for (int j = 0; j < to_draw; j++) {
          put_pixel_modex(draw_x + j, draw_y, value);
        }
      }

      count -= to_draw;
      draw_x += to_draw;

      if (draw_x >= pos_x + width) {
        draw_x = pos_x;
        draw_y++;
      }

      if (draw_y >= pos_y + height) {
        return;
      }
    }
  }
}
