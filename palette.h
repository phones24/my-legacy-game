#pragma once

#define PALETTE_SIZE 256

typedef struct
{
  char r;
  char g;
  char b;
} RGB;

RGB *palette;

RGB *read_palette(const char *filename)
{
  FILE *file = fopen(filename, "r");

  if (!file)
  {
    printf("Error: Could not open palette file %s\n", filename);
    exit(1);
  }

  char header[10];
  if (!fgets(header, sizeof(header), file))
  {
    printf("Error: Could not read the file header\n");
    fclose(file);
    exit(1);
  }

  strip_newline(header);

  if (strcmp(header, "JASC-PAL") != 0)
  {
    printf("Error: Invalid JASC PAL file (missing header)\n");
    fclose(file);
    exit(1);
  }

  char version[6];
  if (!fgets(version, sizeof(version), file))
  {
    printf("Error: Could not read version from palette file\n");
    fclose(file);
    exit(1);
  }

  strip_newline(version);

  int num_colors;
  if (fscanf(file, "%d", &num_colors) != 1 || num_colors != PALETTE_SIZE)
  {
    printf("Error: Invalid or unsupported number of colors (expected %d)\n", PALETTE_SIZE);
    fclose(file);
    exit(1);
  }

  RGB *pal = (RGB *)malloc(num_colors * sizeof(RGB));
  if (!pal)
  {
    printf("Error: Memory allocation failed for the palette\n");
    fclose(file);
    exit(1);
  }

  fgetc(file);

  for (int i = 0; i < num_colors; i++)
  {
    int r, g, b;
    if (fscanf(file, "%d %d %d", &r, &g, &b) != 3)
    {
      printf("Error: Could not read RGB values for color %d\n", i);
      free(pal);
      fclose(file);
      exit(1);
    }
    pal[i].r = (char)r;
    pal[i].g = (char)g;
    pal[i].b = (char)b;
  }

  fclose(file);

  return pal;
}

void apply_palette(RGB *palette) {
  outp(0x03C8, 0);

  for (int i = 0; i < PALETTE_SIZE; i++) {
    outp(0x03C9, palette[i].r / 4);
    outp(0x03C9, palette[i].g / 4);
    outp(0x03C9, palette[i].b / 4);
  }
}

void init_palette() {
  palette = read_palette("res\\ship3.pal");

  apply_palette(palette);
}

void clear_palette() {
  free(palette);
}
