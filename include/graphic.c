#include "graphic.h"

void put_pixel(int pos_x, int pos_y, int color)
{
    unsigned char* location = (unsigned char*)VGA_GRAPHIC_ADDRESS + 320 * pos_y + pos_x;
    *location = color;
}

void clear_graphic()
{
  int i;
  char *p;

  for(i = 0xa0000; i < 0xaffff; i++)
  {
      p = i;
      *p = 15;   //white color
  }
}

void draw_mouse(int pos_x, int pos_y, int color)
{
  //int color = 1; // blue color

  draw_rect(pos_x - 4, pos_y, 10, 2, color);

  draw_rect(pos_x, pos_y - 4, 2, 10, color);
}

void draw_rect(int pos_x, int pos_y, int width, int height, int color)
{
    int i = 0, j = 0;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            put_pixel(pos_x + j, pos_y + i, color);
        }
    }
}
