#include "util.h"
#include "vgaTerminal.h"

size_t terminalRow;
size_t terminalColumn;
uint8_t terminalColour;
uint16_t* terminalBuffer = (uint16_t*)VGA_MEMORY;

static inline uint8_t vgaEntryColour(enum vgaColour fg, enum vgaColour bg)
{
    return fg | bg << 4;
}

// Format data
static inline uint16_t vgaEntry(unsigned char uc, uint8_t colour)
{
    return (uint16_t)uc | (uint16_t)(colour << 8);
}

void terminalInitialise(void)
{
    terminalRow = 0;
    terminalColumn = 0;
    terminalColour = vgaEntryColour(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    // Clear terminal
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
	for (size_t x = 0; x < VGA_WIDTH; x++)
	{
	    terminalBuffer[y * VGA_WIDTH + x] = vgaEntry(' ', terminalColour);
	}
    }
}

void terminalSetColour(uint8_t colour)
{
    terminalColour = colour;
}

uint8_t terminalGetColour(void)
{
  return terminalColour;
}
  
void terminalScroll(void)
{
  // TODO: Benchmark terminal scrolling , looks like it would be quite slow
  // TODO: Suggestion for future terminal scrolling implementation: have a pointer to the head of the terminal, and move that. If done right, everything else should move too.
  for (int y = 0; y < VGA_HEIGHT; y++)
  {
    for (int x = 0; x < VGA_WIDTH; x++)
    {
      terminalBuffer[y * VGA_WIDTH + x] = terminalBuffer[(y + 1) * VGA_WIDTH + x];
    }
  }

  // Whatever is calling this function should presumably increment it. Because it can't go past the maximum, we need to decrement it.
  // TODO: Proper implementation of terminal scrolling
  terminalRow--;
}

void terminalPutEntryAt(char c, uint8_t colour, size_t x, size_t y)
{
    terminalBuffer[y * VGA_WIDTH + x] = vgaEntry(c, colour);
}

void terminalPutChar(char c)
{
  // Newline
  if (c == '\n')
  {
    terminalPutEntryAt(' ', terminalColour, terminalColumn, terminalRow);
    terminalColumn = 0;
    if (++terminalRow == VGA_HEIGHT)
    {
      terminalScroll();
    }
  } else
  {   
    terminalPutEntryAt(c, terminalColour, terminalColumn, terminalRow);
    if (++terminalColumn == VGA_WIDTH)
      {
	terminalColumn = 0;
	if (++terminalRow == VGA_HEIGHT)
	  {
	    terminalScroll();
	  }
      }
    terminalPutEntryAt('_', 15, terminalColumn, terminalRow);
  }
}

void terminalWrite(const char* data, size_t size)
{
  for (size_t i = 0; i < size; i++)
  {
    terminalPutChar(data[i]);
  }
}

void terminalWriteStr(const char* data)
{
  terminalWrite(data, strlen(data));
}

void terminalWriteInt(const int data)
{
  char str[32];
  intToAscii(data, str);
  terminalWriteStr(str);
}

void terminalWriteHex(const int data)
{
  long int rem, quot; 
  char hexdecnum[100]; 
  int i = 0, j, temp; 
  quot = data; 

  if (data == 0)
  {
    terminalWriteStr("0x0");
    return;
  }
  
  while(quot != 0) 
  { 
    temp = quot % 16; 

    if(temp < 10)
    {
      temp = temp + 48;
    }
    else
    {
      temp = temp + 55;
    }

    hexdecnum[i++] = temp; 
    quot = quot / 16; 
  }

  char* start = hexdecnum;
  char* end = start + strlen(hexdecnum) - 1;
  char temp2;

  while (end > start)
  {
    // Swaps values of start and end
    temp2 = *start;
    *start = *end;
    *end = temp2;

    start++;
    end--;
  }

  
  // Null terminator
  hexdecnum[i] = '\0';

  terminalWriteStr("0x");

  /* for(j = i - 1; j >= 0; j--) */
  /*   { */
  /*     terminalPutChar(hexdecnum[j]); */
  /*   } */
  terminalWriteStr(hexdecnum);
}
