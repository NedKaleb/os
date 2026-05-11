#ifndef VGA_TERMINAL_H
#define VGA_TERMINAL_H

// Supplied by compiler
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants necessary for VGA
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

// Colours to be used by VGA
enum vgaColour {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

size_t strlen(const char* str);

void terminalInitialise(void);

void terminalSetColour(uint8_t colour);
uint8_t terminalGetColour(void);
void terminalPutEntryAt(char c, uint8_t colour, size_t x, size_t y);
void terminalPutChar(char c);
void terminalWrite(const char* data, size_t size);
void terminalWriteStr(const char* data);
void terminalWriteInt(const int data);
void terminalWriteHex(const int data);

// Include guard
#endif
