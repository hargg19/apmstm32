#include "stm32f10x.h"
#include "7display.h"

// Define MAX7219 commands
#define MAX7219_REG_NOOP        0x00
#define MAX7219_REG_DIGIT0      0x01
#define MAX7219_REG_DIGIT1      0x02
#define MAX7219_REG_DIGIT2      0x03
#define MAX7219_REG_DIGIT3      0x04
#define MAX7219_REG_DIGIT4      0x05
#define MAX7219_REG_DIGIT5      0x06
#define MAX7219_REG_DIGIT6      0x07
#define MAX7219_REG_DIGIT7      0x08
#define MAX7219_REG_DECODEMODE  0x09
#define MAX7219_REG_INTENSITY   0x0A
#define MAX7219_REG_SCANLIMIT   0x0B
#define MAX7219_REG_SHUTDOWN    0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

// Character table for 7-segment display (0-9 and some letters)
static const uint8_t char_table[] = {
    0x7E, //0 0
    0x30, //1 1
    0x6D, //2 2
    0x79, //3 3
    0x33, //4 4
    0x5B, //5 5
    0x5F, //6 6
    0x70, //7 7
    0x7F, //8 8
    0x7B, //9 9
    0x37, //10 H
    0x4F, //11 E
    0x0E, //12 L
    0x4E, //13 C
    0x63, //14 degree
    0x00, //15 blank
    0x01, //16 -
    0x08, //17 _
};

// GPIO Pin Definitions
#define DIN_PIN   GPIO_BSRR_BS12 // PB12
#define CS_PIN    GPIO_BSRR_BS13 // PB13
#define CLK_PIN   GPIO_BSRR_BS14 // PB14

#define DIN_LOW   GPIOB->BSRR = GPIO_BSRR_BR12
#define DIN_HIGH  GPIOB->BSRR = DIN_PIN
#define CS_LOW    GPIOB->BSRR = GPIO_BSRR_BR13
#define CS_HIGH   GPIOB->BSRR = CS_PIN
#define CLK_LOW   GPIOB->BSRR = GPIO_BSRR_BR14
#define CLK_HIGH  GPIOB->BSRR = CLK_PIN

// Send one bit of data
static void MAX7219_SendBit(uint8_t bit) {
    if (bit) {
        DIN_HIGH; // Set DIN high
    } else {
        DIN_LOW; // Set DIN low
    }
    CLK_HIGH; // Set CLK high
    __NOP();__NOP();__NOP(); // Small delay
    CLK_LOW; // Set CLK low
}

// Send one byte of data
static void MAX7219_SendByte(uint8_t data) {
    for (int i = 7; i >= 0; i--) {
        MAX7219_SendBit((data >> i) & 0x01);
    }
}

// Send data to MAX7219
void MAX7219_SendData(uint8_t address, uint8_t data) {
    CS_LOW; // Pull CS low
    MAX7219_SendByte(address); // Send address
    MAX7219_SendByte(data); // Send data
    CS_HIGH; // Pull CS high to latch data
}

// Initialize MAX7219
void MAX7219_Init(void) {
    // Enable GPIOB clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // Configure PB12 (DIN), PB13 (CS), and PB14 (CLK) as output push-pull
    GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_MODE12 |
                    GPIO_CRH_CNF13 | GPIO_CRH_MODE13 |
                    GPIO_CRH_CNF14 | GPIO_CRH_MODE14);

    GPIOB->CRH |= (GPIO_CRH_MODE12_1 | GPIO_CRH_MODE13_1 | GPIO_CRH_MODE14_1);

    // Initialize MAX7219
    MAX7219_SendData(MAX7219_REG_SHUTDOWN, 0x01);    // Shutdown mode off
    MAX7219_SendData(MAX7219_REG_DECODEMODE, 0x00);  // No decode mode
    MAX7219_SendData(MAX7219_REG_SCANLIMIT, 0x07);   // Scan all digits (0-7)
    MAX7219_SendData(MAX7219_REG_INTENSITY, 0x08);   // Medium intensity
    MAX7219_SendData(MAX7219_REG_DISPLAYTEST, 0x00); // Display test off

    // Clear display
    MAX7219_Clear();
}

// Clear the display
void MAX7219_Clear(void) {
    for (uint8_t i = 0; i < 8; i++) {
        MAX7219_SendData(MAX7219_REG_DIGIT0 + i, 0x00);
    }
}

// Display a single character on a specific position (0-7)
void MAX7219_DisplayChar(uint8_t position, uint8_t character) {
    if (position < 8 && character < sizeof(char_table)) {
        MAX7219_SendData(MAX7219_REG_DIGIT0 + position, char_table[character]);
    }
}
