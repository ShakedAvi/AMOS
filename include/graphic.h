#ifndef GRAPHIC_H
#define GRAPHIC_H

#define VGA_GRAPHIC_ADDRESS 0xA0000

// define our structure
typedef struct __attribute__ ((packed)) {
   unsigned short di, si, bp, sp, bx, dx, cx, ax;
   unsigned short gs, fs, es, ds, eflags;
} regs16_t;

// tell compiler our int32 function is external
extern void rint32(unsigned char intnum, regs16_t *regs);

void put_pixel(int pos_x, int pos_y, int color);
void clear_graphic();
void draw_mouse(int pos_x, int pos_y, int color);
void draw_rect(int pos_x, int pos_y, int width, int height, int color);

#endif
