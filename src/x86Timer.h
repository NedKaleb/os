#ifndef X86_TIMER_H
#define X86_TIMER_H

#include "x86Interrupts.h"

void timerHz(unsigned int hz);
void timerHandler(registers_t r);
void timerSetup();
void timerWait(unsigned int ticks);
unsigned int getTimerTicks();

#endif
