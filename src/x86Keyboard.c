#include <stdint.h>

#include "x86Keyboard.h"
#include "x86io.h"
#include "x86Interrupts.h"
#include "vgaTerminal.h"
#include "util.h"

// US keyboard layout scancode table
// TODO: Add more scancodes (replaces '0's)
// TODO: Key status variables (1 bit for shift, alt, etc.)
unsigned char kbdUS[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	// 9 
  '9', '0', '-', '=', '\b',	// Backspace 
  '\t',			// Tab 
  'q', 'w', 'e', 'r',	// 19 
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	// Enter key 
    0,			// 29   - Control 
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	// 39 
 '\'', '`',   0,		// Left shift 
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			// 49 
  'm', ',', '.', '/',   0,				// Right shift 
  '*',
    0,	// Alt 
  ' ',	// Space bar 
    0,	// Caps lock 
    0,	// 59 - F1 key ... > 
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	// < ... F10 
    0,	// 69 - Num lock
    0,	// Scroll Lock 
    0,	// Home key 
    0,	// Up Arrow 
    0,	// Page Up 
  '-',
    0,	// Left Arrow 
    0,
    0,	// Right Arrow 
  '+',
    0,	// 79 - End key
    0,	// Down Arrow 
    0,	// Page Down 
    0,	// Insert Key 
    0,	// Delete Key 
    0,   0,   0,
    0,	// F11 Key 
    0,	// F12 Key 
    0,	// All other keys are undefined 
};		

void keyboardHandler(registers_t r)
{
  uint8_t scancode;
  // Read from the keyboard's data buffer
  scancode = inb(0x60);

  if (scancode & 0x80)
  {
    // Shift, alt or ctrl has been released
  }
  else
  {
    // A key was just pressed, holding a key down will trigger repeated interrupts
    terminalSetColour(1);
    terminalPutChar(kbdUS[scancode]);
  }
}

void keyboardSetup()
{
  isrRegisterHandler(1, keyboardHandler);
}
