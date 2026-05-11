#include <stdint.h>
#include <stddef.h>

#include "util.h"

void intToAscii(uint32_t num, char* str)
{
  int i, sign;
  // Changes num to positive if it is negative, stores original value in sign
  if ((sign = num) < 0) num = -num;
  i = 0;

  // Sets str[i] to the remainder of the current value of num
  do
  {
    str[i++] = num % 10 + '0';
  } while ((num /= 10) > 0);

  if (sign < 0) str[i++] = '-';
  str[i] = '\0';
  
  // Now we have a reversed integer. The simplest thing to do is to flip it around again.
  char* start = str;
  char* end = start + strlen(str) - 1;
  char temp;

  while (end > start)
  {
    // Swaps values of start and end
    temp = *start;
    *start = *end;
    *end = temp;

    start++;
    end--;
  }
  
}


size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
	len++;
    return len;
}

void* memset(void* startLocation, int val, size_t len)
{
  uint8_t* point = startLocation;

  while (len > 0)
  {
    *point = val;
    point++;
    len--;
  }

  return startLocation;
}
