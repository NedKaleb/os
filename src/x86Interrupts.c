#include <stdint.h>

#include "x86GDT.h"
#include "x86Interrupts.h"
#include "vgaTerminal.h"
#include "util.h"
#include "x86io.h"

isr_t interruptHandlers[256];

idtEntry_t idt[IDT_ENTRIES];
idtRegister_t idtReg;

// This function is taken from https://github.com/cfenollosa/os-tutorial/blob/master/18-interrupts/cpu/isr.c#L8 because I don't want to write this myself
// Much of this interrupt handler also takes inspiration from their work
// Can't do this with a loop because we need the address of the function names
void isrInstall() {
    setIdtGate(0, (intptr_t)isr0);
    setIdtGate(1, (intptr_t)isr1);
    setIdtGate(2, (intptr_t)isr2);
    setIdtGate(3, (intptr_t)isr3);
    setIdtGate(4, (intptr_t)isr4);
    setIdtGate(5, (intptr_t)isr5);
    setIdtGate(6, (intptr_t)isr6);
    setIdtGate(7, (intptr_t)isr7);
    setIdtGate(8, (intptr_t)isr8);
    setIdtGate(9, (intptr_t)isr9);
    setIdtGate(10, (intptr_t)isr10);
    setIdtGate(11, (intptr_t)isr11);
    setIdtGate(12, (intptr_t)isr12);
    setIdtGate(13, (intptr_t)isr13);
    setIdtGate(14, (intptr_t)isr14);
    setIdtGate(15, (intptr_t)isr15);
    setIdtGate(16, (intptr_t)isr16);
    setIdtGate(17, (intptr_t)isr17);
    setIdtGate(18, (intptr_t)isr18);
    setIdtGate(19, (intptr_t)isr19);
    setIdtGate(20, (intptr_t)isr20);
    setIdtGate(21, (intptr_t)isr21);
    setIdtGate(22, (intptr_t)isr22);
    setIdtGate(23, (intptr_t)isr23);
    setIdtGate(24, (intptr_t)isr24);
    setIdtGate(25, (intptr_t)isr25);
    setIdtGate(26, (intptr_t)isr26);
    setIdtGate(27, (intptr_t)isr27);
    setIdtGate(28, (intptr_t)isr28);
    setIdtGate(29, (intptr_t)isr29);
    setIdtGate(30, (intptr_t)isr30);
    setIdtGate(31, (intptr_t)isr31);

    // Remap the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0); 

    // Install the IRQs
    setIdtGate(32, (intptr_t)irq0);
    setIdtGate(33, (intptr_t)irq1);
    setIdtGate(34, (intptr_t)irq2);
    setIdtGate(35, (intptr_t)irq3);
    setIdtGate(36, (intptr_t)irq4);
    setIdtGate(37, (intptr_t)irq5);
    setIdtGate(38, (intptr_t)irq6);
    setIdtGate(39, (intptr_t)irq7);
    setIdtGate(40, (intptr_t)irq8);
    setIdtGate(41, (intptr_t)irq9);
    setIdtGate(42, (intptr_t)irq10);
    setIdtGate(43, (intptr_t)irq11);
    setIdtGate(44, (intptr_t)irq12);
    setIdtGate(45, (intptr_t)irq13);
    setIdtGate(46, (intptr_t)irq14);
    setIdtGate(47, (intptr_t)irq15);
    
    setIdt(); // Load with ASM
}

// To print the message which defines every exception
char* exceptionMessages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isrHandler(registers_t r)
{
  terminalSetColour(1);
  terminalWriteStr("Interrupt: ");
  char str[3];
  intToAscii(r.intNo, str);
  terminalWriteStr(str);
  terminalWriteStr("\n");
  terminalWriteStr(exceptionMessages[r.intNo]);
  terminalWriteStr("\n");
}

void setIdt()
{
  idtReg.base = (intptr_t)&idt;
  idtReg.limit = IDT_ENTRIES * sizeof(idtEntry_t) - 1;
  char str[256];
  intToAscii(idtReg.base, str);
  terminalWriteStr("\nidtReg.base: ");
  terminalWriteStr(str);
  terminalWriteStr("\n");
  // &idtReg must be loaded, not &idt
  __asm__ __volatile__("lidt %0" : : "m" (idtReg));
  __asm__ ("sti");
  // May need to do something to the registers, like in gdtFlush
}

// Return the contents of the IDT register
idtRegister_t viewIdt()
{
  idtRegister_t ret;
  __asm__ __volatile("sidt %0" : "=m" (ret));
  return ret;
}

void setIdtGate(int n, uint32_t handler)
{
  char str[32];
  terminalWriteStr("setIdtGate(");
  intToAscii(n, str);
  terminalWriteStr(str);
  terminalWriteStr(", ");
  intToAscii(handler, str);
  terminalWriteStr(str);
  terminalWriteStr(")");
    
  idt[n].lowOffset = (uint16_t)((handler) & 0xFFFF); // Sets lowOffset to lower half of handler address
  idt[n].sel = IDT_CODE_SEGMENT;
  idt[n].always0 = 0;
  idt[n].flags = 0x8E;
  idt[n].highOffset = (uint16_t)(((handler) >> 16) & 0xFFFF); // Sets high offfset to high half of handler address
}

void isrRegisterHandler(uint8_t n, isr_t handler)
{
  char str[256];
  intToAscii(n, str);
  terminalWriteStr("isrRegisterHandler()");
  terminalWriteStr(str);
  interruptHandlers[n + 32] = handler;
}

void irqHandler(registers_t r)
{
  // Do NOT remove char str[256], and the call to intToAscii(), it will break
  // TODO: Find out why removing this makes it break
  char str[256];
  intToAscii(r.intNo, str);

  #ifdef OUTPUT_IRQS_TO_VGA
  terminalSetColour(1);
  terminalWriteStr("Interrupt: ");
  terminalWriteStr(str);
  terminalWriteStr(" ");
  #endif
  
  if (interruptHandlers[r.intNo] != 0)
  {
    #ifdef OUTPUT_IRQS_TO_VGA
    terminalWriteStr("Calling interrupt...");
    #endif
    
    isr_t handler = interruptHandlers[r.intNo];
    handler(r);
  }
  
  // After each interrupt, an EOI needs to be sent to the PICs, so they can send another interrupt again later
  if (r.intNo >= 40) outb(0xA0, 0x20);
  outb(0x20, 0x20);
}
