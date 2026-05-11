#ifndef PHYSICAL_MEMORY_MANAGER_H
#define PHYSICAL_MEMORY_MANAGER_H

#include <stdint.h>

#include "util.h"
#include "multiboot.h"

#define BLOCK_SIZE      4096  // 1 block of memory is 4kb
#define BLOCKS_PER_BYTE 8     // Using a bitmap

// Defined in getMemoryMap (asm)
#define ADDRESS_OF_NUMBER_OF_SMAP_ENTRIES 0x8500
#define START_ADDRESS_OF_SMAP_ENTRIES 0x8504

static uint32_t* memoryMap = 0;
static uint32_t maxBlocks  = 0;
static uint32_t usedBlocks = 0;

// SMAP entry in the format given by the int 0x15 call in getMemoryMap (asm)
typedef struct  smapEntry
{
  uint64_t baseAddress;
  uint64_t length;
  uint32_t type;
  uint32_t acpi;
} __attribute__ ((packed)) smapEntry_t;

void setBlock(uint32_t bit);
void freeBlock(uint32_t bit);
uint8_t checkBlock(uint32_t bit);
int32_t findFreeBlocks(uint32_t numBlocks);

void initialiseMemoryManager(uint32_t size, uint32_t startAddress);
void printPhysicalMemoryInfo(void);

void initMemory(multiboot_info_t* mboot);

#endif
