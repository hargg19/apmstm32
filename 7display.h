#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// Initialize the MAX7219 module
void MAX7219_Init(void);

// Send data to a specific register
void MAX7219_SendData(uint8_t address, uint8_t data);

// Clear the display
void MAX7219_Clear(void);

// Display a single character on a specific position
void MAX7219_DisplayChar(uint8_t position, uint8_t character);

#endif // DISPLAY_H
