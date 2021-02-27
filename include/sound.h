#ifndef SOUND_H
#define SOUND_H

#include "types.h"
#include "system.h"
#include "task.h"

static void play_sound(uint32 nFrequence);
static void nosound();
void beep(uint32 nFrequence);
void play_mission_impossible();

#endif
