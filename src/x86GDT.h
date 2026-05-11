#ifndef X86GDT_H
#define X86GDT_H

#include <stdint.h>

#define IDT_CODE_SEGMENT 0x08
// If you need to change this constant also change the line below the comment "IDT_DATA_SEGMENT, do NOT remove this comment" in x86Interrupts_asm.s
#define IDT_DATA_SEGMENT 0x10

// I initially had a version of this GDT written in assembly, predominatly based off of the documentation on the OSDev Wiki, but then I found out I could just do it in C - this version is largely based off of http://www.osdever.net/bkerndev/Docs/gdt.htm

// Limit is length of segment, will be set to maximum value of 0xfffff, 20 bits long, split into multiple variables
// Base is start address, will be set to 0, 32 bits long, split into multiple variables
struct gdtEntry
{
  // Kernel mode segment limit (first 16 bits)
  uint16_t limitLower;
  // Kernel mode segment base (first 24 bits, in two different variables)
  uint16_t baseLower;
  uint8_t baseMiddle;
  /* Flags
   * First bit is alwas set to 1 to show that there is a segment present
   * Next two bits are the privilege of the segment, 00 represents the highest
   * Then it's the type bit - 1 for the code segment, 0 for data segment
   * Then, the next bit shows whether this segment contains code - yes 1, no 0
   * The next bit is whether or not the code can be executed from a lower privileged segment - yes 1, no 0
   * Second last bit shows whether the segment is readable - yes 1, no 0
   * Final bit is whether the segment should be managed by the CPU - yes 1, no 0 */
  uint8_t accessFlags;
  /* More flags
   * First bit refers to the granularity, which multiplies the limit by 0x1000, giving us access to 4GB of memory - yes 1, no 0
   * Next bit shows whether the segment is using 32 bit memory - yes 1, no 0
   * The next two are not being used right now
   * TODO: Research more about GDT flags
   * The final four bits of this flag represent the final four bits of the limit */
  uint8_t granularityFlags;
  // Final byte of limit
  uint8_t baseUpper;
} __attribute__((packed));

// Will contain the start address and length of the GDT
struct gdtPtr
{
  uint16_t limit;
  intptr_t base;
} __attribute__((packed));

// Assembly language function prototypes
extern void gdtFlush();

// Function prototypes
void gdtInstall();

#endif
