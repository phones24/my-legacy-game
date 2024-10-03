#include <conio.h>
#include <dos.h>

#include "keyboard.h"

#define KEYBOARD_IRQ 0x09

volatile KEYS keys;
volatile char all_keys[0xff] = { 0 };

void interrupt (*old_keyboard_isr)(void);

void interrupt new_keyboard_isr()
{
  char scan_code = inp(0x60);

  if(scan_code & 128) {
    all_keys[scan_code & 127] = 0;
  } else {
    all_keys[scan_code] = 1;
  }

  keys.p = all_keys[0x19];
  keys.space = all_keys[0x39];
  keys.up = all_keys[0x48];
  keys.down = all_keys[0x50];
  keys.left = all_keys[0x4B];
  keys.right = all_keys[0x4D];
  keys.escape = all_keys[0x01];

  outp(0x20, 0x20);

  (*old_keyboard_isr)();
}

void init_keyboard()
{
  old_keyboard_isr = _dos_getvect(KEYBOARD_IRQ);
  _dos_setvect(KEYBOARD_IRQ, new_keyboard_isr);

  keys.up = 0;
  keys.down = 0;
  keys.left = 0;
  keys.right = 0;
  keys.escape = 0;
  keys.space = 0;
}

void restore_keyboard()
{
  _dos_setvect(KEYBOARD_IRQ, old_keyboard_isr);
}
