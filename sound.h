#pragma once

#include "midasdll.h"

extern MIDASsample sound_ship_shot;
extern MIDASsample sound_ship_expl;
extern MIDASsample sound_enemy_expl;

void midas_error();
void init_sound();
void clear_sound();
void play_sound(MIDASsample, int);
