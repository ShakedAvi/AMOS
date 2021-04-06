#include "sound.h"

/*
  The function plays sound using built in speaker.

  Input:
    The frequency of the sound to play.
  Output:
    None.
*/
static void play_sound(uint32 nFrequence)
{
  int scale;
  if (nFrequence == 0)
  {
    outportb(0x61, inportb(0x61) & ~3);
    return;
  }
  scale = 1193046 / nFrequence;
  outportb(0x43, 0xb6);
  outportb(0x42, scale & 0xff);
  outportb(0x42, scale >> 8);
  outportb(0x61, inportb(0x61) | 3);
}

/*
  The function stops the playing of a sound in the built in speaker.

  Input:
    None.
  Output:
    None.
*/
static void nosound()
{
	uint8 tmp = inportb(0x61) & 0xFC;

	outportb(0x61, tmp);
}

/*
  The function plays a fixed length beep using the built in speaker.

  Input:
    The frequency of the beep to play.
  Output:
    None.
*/
void beep(uint32 nFrequence)
{
	 play_sound(nFrequence);
	 sleep();
	 nosound();
}

/*
  The function plays the theme of the movie "mission impossible"
  with beeps:)

  Input:
    None.
  Output:
    None.
*/
void play_mission_impossible()
{
  beep(784);
  sleep(); sleep(); sleep(); sleep();
  beep(784);
  sleep(); sleep(); sleep(); sleep();
  beep(932);
  sleep(); sleep(); sleep();
  beep(1047);
  sleep(); sleep(); sleep();
  beep(784);
  sleep(); sleep(); sleep(); sleep();
  beep(784);
  sleep(); sleep(); sleep(); sleep();
  beep(699);
  sleep(); sleep(); sleep();
  beep(740);
  sleep(); sleep(); sleep();
  beep(784);
  sleep(); sleep(); sleep(); sleep();
  beep(784);
  sleep(); sleep(); sleep(); sleep();
  beep(932);
  sleep(); sleep(); sleep();
  beep(1047);
  sleep(); sleep(); sleep();
  beep(784);
  sleep(); sleep(); sleep(); sleep();
  beep(784);
  sleep(); sleep(); sleep(); sleep();
  beep(699);
  sleep(); sleep(); sleep();
  beep(740);
  sleep(); sleep(); sleep();
  beep(932);
  sleep();
  beep(784);
  sleep();
  beep(5870);
  sleep(); sleep(); sleep(); sleep(); sleep(); sleep(); sleep(); sleep();
  beep(932);
  sleep();
  beep(784);
  sleep();
  beep(554);
  sleep(); sleep(); sleep(); sleep(); sleep(); sleep(); sleep(); sleep();
  beep(932);
  sleep();
  beep(784);
  sleep();
  beep(523);
  sleep(); sleep(); sleep(); sleep(); sleep(); sleep(); sleep(); sleep();
  beep(466);
  sleep();
  beep(523);
}
