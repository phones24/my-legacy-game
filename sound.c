#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

#include "midasdll.h"
#include "timer.h"

MIDASsample sound_ship_shot;
MIDASsample sound_ship_expl;
MIDASsample sound_enemy_expl;

void midas_error(void) {
  set_mode_03h();

  int error = MIDASgetLastError();

  printf("\nMIDAS error: %s\n", MIDASgetErrorMessage(error));

  if (!MIDASclose()) {
    printf("\nBIG PANIC! MIDASclose Failed: %s\n", MIDASgetErrorMessage( MIDASgetLastError()));
  }

  exit(1);
}

void init_sound() {
  if (!MIDASstartup()) {
    midas_error();
  }

  if (!MIDASdetectSoundCard()) {
    if (!MIDASconfig()) {
      midas_error();
    }
  }

  if (!MIDASinit()) {
    midas_error();
  }

  if (!MIDASsetTimerCallbacks(TICKS_PER_SECOND * 1000, 0, &timer_callback, NULL, NULL)) {
  // if (!MIDASsetTimerCallbacks(1000, 0, &timer_callback, NULL, NULL)) {
    midas_error();
  }

  if (!MIDASopenChannels(50)) {
    midas_error();
  }

  if (!MIDASallocAutoEffectChannels(10)) {
    midas_error();
  }

  if ( (sound_ship_shot = MIDASloadWaveSample("res\\lshot2.wav", MIDAS_LOOP_NO)) == 0) {
    midas_error();
  }

  if ( (sound_ship_expl = MIDASloadWaveSample("res\\expl2.wav", MIDAS_LOOP_NO)) == 0) {
    midas_error();
  }

  if ( (sound_enemy_expl = MIDASloadWaveSample("res\\expl1.wav", MIDAS_LOOP_NO)) == 0) {
    midas_error();
  }

  // if (!MIDASsetMusicMode(MIDAS_AUTODETECT)) {
  //   printf("\nMIDAS set music mode failed: %s\n", MIDASgetErrorMessage( MIDASgetLastError()));
  //   exit(1);
  // }
}

void clear_sound() {
  if (!MIDASfreeSample(sound_ship_shot) ) {
    midas_error();
  }

  if (!MIDASfreeSample(sound_ship_expl) ) {
    midas_error();
  }

  if (!MIDASfreeSample(sound_enemy_expl) ) {
    midas_error();
  }

  if (!MIDASstopBackgroundPlay() ) {
    midas_error();
  }

  if (!MIDASremoveTimerCallbacks()) {
    midas_error();
  }

  if ( !MIDASclose() ) {
    midas_error();
  }
}

void play_sound(MIDASsample sample, int volume) {
  if (MIDASplaySample(sample, MIDAS_CHANNEL_AUTO, 0, 22050, volume, MIDAS_PAN_MIDDLE) == 0) {
    midas_error();
  }
}
