#include "mouse.h"

static int8 mouse_byte[3];
static int8 offset = 0;

static int32 mouse_x = 160;
static int32 mouse_y = 100;
static int32 mcolor = 1;

static int32 win_x = 50;
static int32 win_y = 50;
static int32 win_color = 3;

void mouse_handler()
{
  char printMe[10] = { 0 };
  int8 x_diff = 0, y_diff = 0, test = 0;
  int8 status = inportb(0x64);

  int8 input = inportb(0x60);

  if (status & 0x20)
  {
    switch(offset)
    {
      case 0:
        mouse_byte[0] = input;
        if (!(input & 0x08)) return;
        offset++;
        break;
      case 1:
        mouse_byte[1] = input;
        offset++;
        break;
      case 2:
        mouse_byte[2] = input;

        if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40)
        {
						/* x/y overflow? bad packet! */
            offset = 0;
						break;
				}

        if (mouse_byte[0] & 0x01) // Left Click
        {
            mcolor = 4;
            offset = 0;
            break;
				}
				if (mouse_byte[0] & 0x02) // Right Click
        {
            mcolor = 5;
            offset = 0;
            break;
				}
				if (mouse_byte[0] & 0x04) // Middle Click
        {
            mcolor = 6;
            offset = 0;
            break;
        }


        x_diff = mouse_byte[1] - ((mouse_byte[0] << 4) & 0x100);
        y_diff = mouse_byte[2] - ((mouse_byte[0] << 3) & 0x100);

        mouse_x += mouse_byte[1];// - ((mouse_byte[0] << 4) & 0x100);
        mouse_y -= mouse_byte[2];// - ((mouse_byte[0] << 3) & 0x100);

        if(mcolor == 4 && mouse_x > win_x && mouse_x < (win_x + 100)
          && mouse_y > win_y && mouse_y < (win_y + 50) && mouse_x < 320 && mouse_x > 0
          && mouse_y > 0 && mouse_y < 200)
        {
          win_x += mouse_byte[1];
          win_y -= mouse_byte[2];
        }

        if(mouse_x < 0)
        {
          mouse_x = 0;
        }
        if(mouse_y < 0)
        {
          mouse_y = 0;
        }
        if(mouse_x > 320)
        {
          mouse_x = 320;
        }
        if(mouse_y > 200)
        {
          mouse_y = 200;
        }

        /*print("Mouse x: ");
        itoa(mouse_x, printMe, 10);
        print(printMe);
        print(" Mouse y: ");
        itoa(mouse_y, printMe, 10);
        print(printMe);
        print("\n");*/

        clear_graphic();
        draw_rect(win_x, win_y, 100, 50, win_color);
        draw_mouse((uint32)(mouse_x), (uint32)(mouse_y), mcolor);

        offset = 0;
        break;
      }
  }
}

void mouse_wait(uint8 a_type)
{
  uint32 time_out = 100000;
  if(a_type == 0)
  {
    while(time_out--) //Data
    {
      if((inportb(MOUSE_STATUS) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(time_out--) //Signal
    {
      if((inportb(MOUSE_STATUS) & 2) == 0)
      {
        return;
      }
    }
    return;
  }
}

void mouse_write(uint8 a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outportb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outportb(0x60, a_write);
}

uint8 mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inportb(0x60);
}

void init_mouse()
{
    uint8 status;

    //Enable the auxiliary mouse device
    mouse_wait(1);
    outportb(0x64, 0xA8);

    //Enable the interrupts
    mouse_wait(1);
    outportb(0x64, 0x20);
    mouse_wait(0);
    status = (inportb(0x60) | 2);
    mouse_wait(1);
    outportb(0x64, 0x60);
    mouse_wait(1);
    outportb(0x60, status);

    //Tell the mouse to use default settings
    mouse_write(0xF6);
    mouse_read();  //Acknowledge

    //Enable the mouse
    mouse_write(0xF4);
    mouse_read();  //Acknowledge

    register_interrupt_handler(44, &mouse_handler);
}
