#include "vgaTerminal.h"
#include "util.h"
#include "x86GDT.h"
#include "x86Interrupts.h"
#include "x86Timer.h"
#include "x86io.h"
#include "x86Scheduler.h"
#include "x86Keyboard.h"
#include "x86Speaker.h"
#include "physicalMemoryManager.h"
#include "multiboot.h"

typedef struct multibootMemoryMap
{
  uint32_t size;
  uint32_t baseAddressLow, baseAddressHigh;
  uint32_t lengthLow, lengthHigh;
  uint32_t type;
} multibootMemoryMap_t;

int task()
{
  terminalWriteStr("Task\n");
  
  return 0;
}

void kMain(multiboot_info_t* mboot, uint32_t mbootMagicNumber)
{
  /* Make sure the magic number matches for memory mapping*/
  if(mbootMagicNumber != MULTIBOOT_BOOTLOADER_MAGIC)
  {
    terminalWriteStr("Invalid magic number: ");
    terminalWriteHex(mbootMagicNumber);
    while (true);
  }
  
    /* Check bit 6 to see if we have a valid memory map */
    if(!(mboot->flags >> 6 & 0x1)) {
        terminalWriteStr("Invalid memory map given by GRUB bootloader\n");
    }
  
  timerSetup();
  keyboardSetup();
  terminalSetColour(2);
  
  /* for (int i = 0; i < 80; i++) */
  /* { */
  /*   terminalWriteStr("make"); */
  /*   terminalWriteStr("\n"); */
  /* } */

  char str[256];
  
  idtRegister_t idtOutput = viewIdt();
  intToAscii(idtOutput.base, str);
  terminalWriteStr("idtOutput.base: ") ;
  terminalWriteStr(str);
  terminalWriteStr("\n");
  idtEntry_t* startOfInterruptTable = (idtEntry_t*)((intptr_t)idtOutput.base);
  intToAscii((*startOfInterruptTable).sel, str);
  terminalWriteStr("*startOfInterruptTable.sel: ");
  terminalWriteStr(str);
  terminalWriteStr("\n");

  /* isr_t handler = (isr_t)(intptr_t)((*startOfInterruptTable).lowOffset || (*startOfInterruptTable).highOffset); */
  int address = (*startOfInterruptTable).lowOffset + ((*startOfInterruptTable).highOffset << 16);
  intToAscii(address, str);
  terminalWriteStr(str);

  intToAscii(sizeof(idtRegister_t), str);
  terminalWriteStr("\nsizeof(struct gdtEntry): ");
  terminalWriteStr(str);
  terminalWriteStr("\n");
  
  for (int i = 0; i < 100000; i++) {}
  terminalWriteStr("\n1");

  //boop();
  
  /* while (true) */
  /* { */
  /*   timerWait(99); */
  /*   terminalWriteStr("Test"); */
  /* } */

  /* scheduleProcess(&task); */
  
  /* timerWait(100); */
  /* terminalSetColour(2); */
  /* terminalWriteStr("2"); */
  /* runNextProcess(); */

  terminalWriteStr("Hello\n");
  timerWait(100);
  terminalWriteStr("World\n");

  terminalWriteStr("\n");
  initMemory(mboot);
  
  while (true) { asm("hlt"); } 
  
  /* int x = 0; */
  
  /* while (true) */
  /* { */
  /*   for (int i = 0; i < 10000000; i++) {} */
    
  /*   intToAscii(getTimerTicks(), str); */
  /*   terminalWriteStr(str); */
 
  /*   x++; */
  /*   terminalSetColour(x % 16); */
  /*   terminalWriteStr(str); */
  /* } */
}
