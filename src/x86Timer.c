#include "x86Timer.h"
#include "x86io.h"
#include "x86Interrupts.h"
#include "vgaTerminal.h"
#include "util.h"

// 1.19MHz
#define PIT_HZ 1193180

unsigned int totalTimerTicks = 0;

// Sets the timer to a given frequency
void timerHz(unsigned int hz)
{
  int divisor = PIT_HZ / hz;
  // The "command register" for the PIT is accessed through port 0x43
  // This creates a square-wave with order Least Significant Byte then Most Significant Byte
  // TODO: add link to documentation when the internet is working again
  outb(0x43, 0x36);
  // 0x40 is the data register of the PIT channel 0, which is the general purpose one
  // Sets the divisor
  outb(0x40, divisor & 0xFF);
  outb(0x40, divisor >> 8);
}

// Called everytime the PIT triggers an interrupt
void timerHandler(registers_t r)
{
  totalTimerTicks++;

  // Every 100 ticks, output to screen
  #ifdef OUTPUT_PIT_TICKS_TO_VGA
  if (totalTimerTicks % 100 == 0)
  {
    char str[256];
    intToAscii(totalTimerTicks, str);
    terminalWriteStr("100 ticks");    
  }
  #endif
}

void timerSetup()
{
  // Installs timerHandler() to IRQ0
  timerHz(250);
  isrRegisterHandler(0, timerHandler);
  terminalSetColour(1);
  terminalWriteStr("Timer setup");
  terminalSetColour(0);
}

void timerWait(unsigned int ticks)
{
  unsigned long eticks = ticks + totalTimerTicks;

  while (totalTimerTicks < eticks)
  {
    // Do NOT remove this, for some reason, if the colour isn't changed, it will never exit
    // TODO: Find out why timerWait() breaks without redundant code (optimise)
    uint8_t colour = terminalGetColour();
    terminalSetColour(colour);
  }
}

unsigned int getTimerTicks()
{
  return totalTimerTicks;
}
