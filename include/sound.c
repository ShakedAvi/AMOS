#include "sound.h"

//Play sound using built in speaker
static void play_sound(uint32 nFrequence) {
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

//Make it stop beeping
static void nosound()
{
	uint8 tmp = inportb(0x61) & 0xFC;

	outportb(0x61, tmp);
}

//Make a beep
void beep(uint32 nFrequence)
{
	 play_sound(nFrequence);
	 sleep();
	 nosound();
}

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
