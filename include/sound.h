#ifndef SOUND_H
#define SOUND_H

#include "types.h"
#include "system.h"
#include "task.h"

/*
  The function plays sound using built in speaker.

  Input:
    The frequency of the sound to play.
  Output:
    None.
*/
static void play_sound(uint32 nFrequence);

/*
  The function stops the playing of a sound in the built in speaker.

  Input:
    None.
  Output:
    None.
*/
static void nosound();

/*
  The function plays a fixed length beep using the built in speaker.

  Input:
    The frequency of the beep to play.
  Output:
    None.
*/
void beep(uint32 nFrequence);

/*
  The function plays the theme of the movie "mission impossible"
  with beeps:)

  Input:
    None.
  Output:
    None.
*/
void play_mission_impossible();

#endif
