#include "x86io.h"

// Taken from OSDev Wiki

inline void outb(uint16_t port, uint8_t val)
{
  // If something seems to be wrong with this, it's probably because it used to be outb instead of out, but the assembler was giving warnings about mnemonics, so I changed it to this
  __asm__ volatile ( "out %0, %1" : : "Nd"(port), "a"(val));
  /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
   * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
   * The  outb  %al, %dx  encoding is the only option for all other cases.
   * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    // See note on outb()
    __asm__ volatile ( "in %0, %1"
                   : "=a"(ret)
                   : "Nd"(port));
    return ret;
}
