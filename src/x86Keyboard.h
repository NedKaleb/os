#ifndef X86KEYBOARD_H
#define X86KEYBOARD_H

#include "x86Interrupts.h"

void keyboardHandler(registers_t r);
void keyboardSetup();

#endif
