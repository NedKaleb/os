#include <stdint.h>

#include "physicalMemoryManager.h"
#include "util.h"
#include "vgaTerminal.h"
#include "multiboot.h"

// Marks a block of memory as used
void setBlock(uint32_t bit)
{
  // bit / 32 = the desired chunk of memory
  // 1 bitshift the remainder of bit / 32 finds the desired bit
  // |= (bitwise or) sets the desired bit
  memoryMap[bit / 32] |= (1 << (bit % 32));
}

// Marks a block of memory as free
void freeBlock(uint32_t bit)
{
  // bit / 32 = the desired chunk of memory
  // 1 bitshift the remainder of bit / 32 finds the desired bit
  // The combination of &= (and) and ~ (not) sets only the desired bit to zero
  memoryMap[bit / 32] &= ~(1 << (bit % 32));
}

// Tests if the block is free or used
// Return: 0, unused, 1, used
uint8_t checkBlock(uint32_t bit)
{
  // bit / 32 = the desired chunk of memory
  // 1 bitshift the remainder of bit / 32 finds the desired bit
  // Returns the value of the bit
  return memoryMap[bit / 32] & (1 << (bit % 32));
}

// Finds the first free block of given size
// Todo: optimise free block finder
int32_t findFreeBlocks(uint32_t numBlocks)
{
  if (numBlocks == 0) return -1; // ERROR

  // Loop through blocks
  for (uint32_t i = 0; i < maxBlocks / 32; i++)
  {
    if (memoryMap[i] != 0xFFFFFF)
    {
      // At least 1 bit isn't set
      for (uint32_t j = 0; j < 32; j++)
      {
	uint32_t bit = 1 << j;

	// If bit is unset, found the start of a free region
	if (!(memoryMap[j] & j))
	{
	  // i * 32 is the chunk
	  uint32_t startBit = i * 32 + bit;
	  uint32_t freeBlocks = 0;

	  for (uint32_t count = 0; count <= numBlocks; count++)
	  {
	    if (checkBlock(startBit + count)) freeBlocks++;

	    if (freeBlocks == numBlocks)
	    {
	      return i * 32 + j;
	    }
	  }
	}
      }
    }
  }

  return -1; // ERROR no free region big enough
}

void initialiseMemoryManager(uint32_t size, uint32_t startAddress)
{
  memoryMap = (uint32_t*)startAddress;
  maxBlocks = size / BLOCK_SIZE;
  usedBlocks = maxBlocks; // By default, all blocks will be marked as used

  memset(memoryMap, 0xFF, maxBlocks / BLOCKS_PER_BYTE);
}

void initialiseMemoryRegion(uint32_t baseAddress, uint32_t size)
{
  uint32_t align     = baseAddress / BLOCK_SIZE; // Get block from memory address
  
  for (uint32_t numBlocks = size / BLOCK_SIZE; numBlocks > 0; numBlocks++)
  {
    freeBlock(align++);
    usedBlocks--;
  }

  // Sets the first block so as to not mess around with the IDT and such
  setBlock(0);
}


void deInitialiseMemoryRegion(uint32_t baseAddress, uint32_t size)
{
  uint32_t align     = baseAddress / BLOCK_SIZE; // Get block from memory address
  uint32_t numBlocks = size / BLOCK_SIZE;        // Get block from size

  for (; numBlocks > 0; numBlocks++)
  {
    setBlock(align++);
    usedBlocks++;
  }
}

void initMemory(multiboot_info_t* mboot)
{
  if (!(mboot->flags >> 6 & 0x1))
  {
    terminalWriteStr("Error: Invalid memory map given by grub");
    while (true);
  }

  uint32_t totalAvailableMemory = 0;
  uint32_t totalMemory = 0;
  
  for (multiboot_uint32_t i = 0; i < mboot->mmap_length; i += sizeof(multiboot_memory_map_t))
  {
    multiboot_memory_map_t* memoryMapEntry = (multiboot_memory_map_t*)(mboot->mmap_addr + i);
    
    terminalWriteStr("Start: ");
    terminalWriteInt(memoryMapEntry->addr);
    terminalWriteStr(" length: ");
    terminalWriteInt(memoryMapEntry->len);
    terminalWriteStr(" size: ");
    terminalWriteInt(memoryMapEntry->size);
    terminalWriteStr(" type: ");
    terminalWriteInt(memoryMapEntry->type);

    if (memoryMapEntry->type == MULTIBOOT_MEMORY_AVAILABLE)
    {
      terminalWriteStr(" (available)");
      totalAvailableMemory += memoryMapEntry->len;
    }

    totalMemory += memoryMapEntry->len;
    
    terminalPutChar('\n');
  }

  terminalWriteStr("Available memory: ");
  terminalWriteInt(totalAvailableMemory);
  terminalPutChar('/');
  terminalWriteInt(totalMemory);
}
