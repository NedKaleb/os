#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

void intToAscii(uint32_t num, char* str);
size_t strlen(const char* str);
void* memset(void* startLocation, int val, size_t len);

#endif
