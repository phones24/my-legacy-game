#include <conio.h>
#include <dos.h>

#include "keyboard.h"

#define KEYBOARD_IRQ 0x09

volatile KEYS keys;

void interrupt (*old_keyboard_isr)(void);

void interrupt new_keyboard_isr()
{
  char scan_code = inp(0x60);

  switch (scan_code)
  {
    case 0x39:
      keys.space = 1;
      break;
    // case 0x11:
    case 0x48:
      keys.up = 1;
      break;
    // case 0x1F:
    case 0x50:
      keys.down = 1;
      break;
    // case 0x1E:
    case 0x4B:
      keys.left = 1;
      break;
    // case 0x20:
    case 0x4D:
      keys.right = 1;
      break;
    case 0x01:
      keys.escape = 1;
      break;

    // case 0x91:
    case 0xB9:
      keys.space = 0;
      break;
    case 0xC8:
      keys.up = 0;
      break;
    // case 0x9F:
    case 0xD0:
      keys.down = 0;
      break;
    // case 0x9E:
    case 0xCB:
      keys.left = 0;
      break;
    // case 0xA0:
    case 0xCD:
      keys.right = 0;
      break;
    case 0x3B:
      keys.escape = 0;
      break;

    default:
        break;
  }

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
