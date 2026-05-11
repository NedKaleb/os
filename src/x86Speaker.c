#include <stdint.h>

#include "x86Speaker.h"
#include "x86Interrupts.h"
#include "x86io.h"
#include "x86Timer.h"

// Will play sound through the PC speaker, by using interrupts of PIT 2 to generate a frequency.
static void playSound(uint32_t frequency)
{
  uint32_t divisor;
  uint8_t tmp;

  // Sets the frequency if PIT 2
  divisor = 1193180 / frequency;
  outb(0x43, 0xB6);
  outb(0x42, (uint8_t)divisor);
  outb(0x42, (uint8_t)(divisor >> 8));

  // Plays the sound
  tmp = inb(0x61);
  if (tmp != (tmp | 3))
  {
    outb(0x61, tmp | 3);
  }
}

static void noSound(void)
{
  outb(0x61, (inb(0x61) & 0xFC));
}

void beep()
{
  playSound(1000);
  timerWait(5);
  noSound();
}

void boop()
{
  for (int i = 0; i < 50; i++)
  {
    playSound(300 - i);
    timerWait(1);
  }

  noSound();
}
