#include "x86GDT.h"
#include "vgaTerminal.h"

struct gdtEntry gdt[3];
struct gdtPtr gp;

// TODO: Research why it is called a GDT gate
// Sets parameters of a specified GDT
void gdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char accessFlags, unsigned char granularityFlags)
{
  // It needs to be done like this because the base is split up into three variables for some reason
  gdt[num].baseLower = (base & 0xFFFF);
  gdt[num].baseMiddle = (base >> 16) & 0xFF;
  gdt[num].baseUpper = (base >> 24) & 0xFF;

  // And so is the limit
  gdt[num].limitLower = (limit & 0xFFFF);
  // Part of the limit is stored with the flags for some reason
  gdt[num].granularityFlags = ((limit >> 16) & 0x0F);

  // Setup flags - see definition of GDTEntry struct for documentation
  gdt[num].granularityFlags |= (granularityFlags & 0xF0);
  gdt[num].accessFlags = accessFlags;
}

// Should be called by main, as it initialises all of the GDT data
void gdtInstall()
{
  // There will be three GDT entries, and the limit has to be set as one less than the size, as it is then used as an offset from the base
  gp.limit = (sizeof(struct gdtEntry) * 3) - 1;
  gp.base = (intptr_t)&gdt;

  // I believe this is necessary because there are required null-bytes at the start of the GDT
  gdtSetGate(0, 0, 0, 0, 0);

  // Code segment - 4GB limit and 32-bit opcodes
  gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
  
  // Data segment - same as code segment but with the type bit set to data instead of code
  gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

  terminalWriteStr("Preparing to flush GDT");
  gdtFlush();
}
