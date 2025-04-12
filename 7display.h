#ifndef _7DISPLAY_H
#define _7DISPLAY_H

#include <stdint.h>
#include "stm32f10x.h"

// Pin Definitions (STM32F103C6T6 - PB12, PB13, PB14)
#define MAX7219_DIN_PIN    12
#define MAX7219_CS_PIN     13
#define MAX7219_CLK_PIN    14
#define MAX7219_PORT       GPIOB

// MAX7219 Register Addresses
#define MAX7219_NOOP       0x00
#define MAX7219_DIGIT0     0x01
#define MAX7219_DIGIT1     0x02
#define MAX7219_DIGIT2     0x03
#define MAX7219_DIGIT3     0x04
#define MAX7219_DIGIT4     0x05
#define MAX7219_DIGIT5     0x06
#define MAX7219_DIGIT6     0x07
#define MAX7219_DIGIT7     0x08
#define MAX7219_DECODE     0x09
#define MAX7219_INTENSITY  0x0A
#define MAX7219_SCAN_LIMIT 0x0B
#define MAX7219_SHUTDOWN   0x0C
#define MAX7219_TEST       0x0F

// Function Prototypes
void SevenSegment_Init(void);
void SevenSegment_Write(uint8_t address, uint8_t data);
void SevenSegment_DisplayNumber(int32_t number);
void SevenSegment_DisplayString(const char* str);
void SevenSegment_SetBrightness(uint8_t brightness);
void SevenSegment_Clear(void);
void SevenSegment_TestMode(uint8_t enable);

#endif // _7DISPLAY_H