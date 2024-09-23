#ifndef RES_H
#define RES_H

#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)

#define PSX_MAGIC 0x00100000  // Example magic number for PSX header

typedef struct {
  char bfType[2];
  unsigned int bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned int bfOffBits;
} BMPFILEHEADER;

typedef struct {
  unsigned int biSize;
  unsigned int biWidth;
  unsigned int biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  unsigned int biCompression;
  unsigned int biSizeImage;
  unsigned int biXPelsPerMeter;
  unsigned int biYPelsPerMeter;
  unsigned int biClrUsed;
  unsigned int biClrImportant;
} BMPIMAGEHEADER;

typedef struct {
  unsigned char manufacturer;
  unsigned char version;
  unsigned char encoding;
  unsigned char bitsPerPixel;
  unsigned int xmin, ymin, xmax, ymax;
  unsigned int hres, vres;
  unsigned char palette[48];
  unsigned char reserved;
  unsigned char colorPlanes;
  unsigned int bytesPerLine;
  unsigned int paletteType;
  unsigned char filler[58];
} PCX_HEADER;

void strip_newline(char *str)
{
  int len = strlen(str);

  if (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
  {
    str[len - 1] = '\0';
  }

  if (len > 1 && str[len - 2] == '\r')
  {
    str[len - 2] = '\0';
  }
}

IMAGE read_bmp(const char *filename)
{
  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    fprintf(stderr, "Error opening BMP file %s\n", filename);
    exit(1);
  }

  IMAGE image;
  BMPFILEHEADER file_header;
  BMPIMAGEHEADER image_header;

  fread(&file_header, sizeof(BMPFILEHEADER), 1, file);
  fread(&image_header, sizeof(BMPIMAGEHEADER), 1, file);

  if (file_header.bfType[0] != 'B' || file_header.bfType[1] != 'M')
  {
    fprintf(stderr, "Not a BMP file, bfType: %s\n", file_header.bfType);
    fclose(file);
    exit(1);
  }

  if (image_header.biBitCount != 8)
  {
    fprintf(stderr, "Only 8-bit BMP files are supported, biBitCount: %x\n", image_header.biBitCount);
    fclose(file);
    exit(1);
  }

  image.width = image_header.biWidth;
  image.height = image_header.biHeight;
  image.data = (char *)malloc(image_header.biWidth * image_header.biHeight);

  fseek(file, file_header.bfOffBits, SEEK_SET);

  unsigned char *row_data = (unsigned char *)malloc(image.width);
  for (int y = 0; y < image.height; y++) {
    int dest_row = image.height - 1 - y;
    fread(row_data, 1, image.width, file);
    memcpy(&image.data[dest_row * image.width], row_data, image.width);
  }

  free(row_data);
  fclose(file);

  return image;
}

IMAGE_RLE read_pcx(const char* filename) {
  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    fprintf(stderr, "Error opening PCX file %s\n", filename);
    exit(1);
  }

  IMAGE_RLE image;
  PCX_HEADER header;

  fread(&header, sizeof(PCX_HEADER), 1, file);

  image.width = header.xmax - header.xmin + 1;
  image.height = header.ymax - header.ymin + 1;

  fseek(file, 0, SEEK_END);

  long file_size = ftell(file);
  if(file_size < 0) {
    printf("Error: Could not get file size: %s\n", filename);
    exit(1);
  }
  fseek(file, sizeof(PCX_HEADER), SEEK_SET);

  image.data_size = file_size - sizeof(PCX_HEADER);
  image.data = (char *)malloc(image.data_size);

  if(image.data == NULL) {
    printf("Error: Could not allocate memory for image data\n");
    exit(1);
  }

  fread(image.data, 1, image.data_size, file);

  fclose(file);

  return image;
}

SPRITE_SHEET read_sprite_sheet(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Error: Could not open file %s\n", filename);
    exit(1);
  }

  SPRITE_SHEET sprite_sheet;

  if(fscanf(file, "%u", &sprite_sheet.count) != 1) {
    printf("Error: Could not read sprite sheet count\n");
    fclose(file);
    exit(1);
  }

  if(fscanf(file, "%u %u", &sprite_sheet.width, &sprite_sheet.height) != 2) {
    printf("Error: Could not read sprite sheet dimensions\n");
    fclose(file);
    exit(1);
  }

  if(sprite_sheet.count == 0 || sprite_sheet.count > 20) {
    printf("Error: Invalid sprite sheet count\n");
    fclose(file);
    exit(1);
  }

  int cnt = 0;
  for (int i = 0; i < sprite_sheet.count; i++) {
    SPRITE_DEF sprite_def;

    if(fscanf(file, "%u %u %u %u", &sprite_def.x0, &sprite_def.y0, &sprite_def.x1, &sprite_def.y1) != 4) {
      printf("Error: Could not read sprite definition\n");
      fclose(file);
      exit(1);
    }

    sprite_sheet.sprites_def[cnt] = sprite_def;
    cnt++;
  }

  fclose(file);

  // printf("count: %u\n", sprite_sheet.count);
  // printf("w, h: %u, %u\n", sprite_sheet.width, sprite_sheet.height);
  // for (int i = 0; i < sprite_sheet.count; i++) {
  //   printf("x0, y0, x1, y1: %u, %u, %u, %u\n", sprite_sheet.sprites_def[i].x0, sprite_sheet.sprites_def[i].y0, sprite_sheet.sprites_def[i].x1, sprite_sheet.sprites_def[i].y1);
  // }

  return sprite_sheet;
}

#endif
