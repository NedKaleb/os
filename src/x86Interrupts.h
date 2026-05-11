#ifndef X86INTERRUPTS_H
#define X86INTERRUPTS_H

#include <stdint.h>

// Struct which aggregates many registers 
typedef struct {
  uint32_t ds; // Data segment selector
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
  uint32_t intNo, errCode; // Interrupt number and error code (if applicable)
  uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically 
} registers_t;

// IDT is Interrupt Descriptor Table

// Pointer to the array of interrupt handlers, can be read with "lidt" assembly instruction https://www.felixcloutier.com/x86/lgdt:lidt
typedef struct
{
  uint16_t limit; // Address to IDT
  uint32_t base; // Size of table in bytes
} __attribute__((packed)) idtRegister_t;

// Function pointer in format void isr_t(registers_t)
typedef void (*isr_t)(registers_t);

// Stores information about interrupt handler
typedef struct
{
  uint16_t lowOffset; // Lower 16 bytes of the address of handler
  uint16_t sel; // Used to select kernel segment
  uint8_t always0; // Basically padding
  /* Bit 7: presence of interrupt
   * Bits 6-5: privillege level of caller, with 0 being kernel and 3 being user
   * Bit 4: set to 0 in an interrupt gate TODO: Research more about interrupt gates
   * Bits 3-0: set to 1110 (decimal 14) to represent a 32-bit interrupt gate */
  uint8_t flags;
  uint16_t highOffset; // Upper 16 bytes of address. I don't know why it is layed out like this
} __attribute__((packed)) idtEntry_t;

// Total number of interrupt handlers it is possible to use - 256 on x86 32-bit
#define IDT_ENTRIES 256

// Function prototypes
void setIdt(void);
idtRegister_t viewIdt(void);
void isrHandler(registers_t r);
void isrInstall(void);
void setIdtGate(int n, uint32_t handler);
void isrRegisterHandler(uint8_t n, isr_t handler);

// ISRs reserved for CPU exceptions
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void idtInit();

#endif
