#include "7display.h"
#include "delay.h"  // Pastikan delay.h sudah diimplementasikan

// Tabel karakter (0-9, A-F, '-', '_')
static const uint8_t charTable[] = {
    0x7E, // 0
    0x30, // 1
    0x6D, // 2
    0x79, // 3
    0x33, // 4
    0x5B, // 5
    0x5F, // 6
    0x70, // 7
    0x7F, // 8
    0x7B, // 9
    0x77, // A
    0x1F, // b
    0x4E, // C
    0x3D, // d
    0x4F, // E
    0x47, // F
    0x01, // -
    0x00, // (kosong)
    0x40, // (tanda minus)
    0x08  // underscore
};

// Inisialisasi GPIO untuk SPI (bit-bang)
static void GPIO_Config(void) {
    // Aktifkan clock GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    
    // Konfigurasi PB12 (DIN), PB13 (CS), PB14 (CLK) sebagai output push-pull, 50MHz
    // CRH (Pin 8-15), CNF=00 (Push-Pull), MODE=11 (50MHz)
    MAX7219_PORT->CRH &= ~(
        (0xF << (4*(MAX7219_DIN_PIN-8)) | 
        (0xF << (4*(MAX7219_CS_PIN-8))) | 
        (0xF << (4*(MAX7219_CLK_PIN-8)))
    );
    
    MAX7219_PORT->CRH |= (
        (0x3 << (4*(MAX7219_DIN_PIN-8))) | 
        (0x3 << (4*(MAX7219_CS_PIN-8))) | 
        (0x3 << (4*(MAX7219_CLK_PIN-8)))
    );
    
    // Set CS high, CLK low
    MAX7219_PORT->BSRR = (1 << MAX7219_CS_PIN);
    MAX7219_PORT->BRR  = (1 << MAX7219_CLK_PIN);
}

// Fungsi SPI 16-bit (bit-bang)
static void SPI_Send16(uint16_t data) {
    // CS low
    MAX7219_PORT->BRR = (1 << MAX7219_CS_PIN);
    
    // Kirim 16-bit data (MSB first)
    for(uint8_t i = 16; i > 0; i--) {
        // Set DIN berdasarkan bit
        if(data & (1 << (i-1)))
            MAX7219_PORT->BSRR = (1 << MAX7219_DIN_PIN);
        else
            MAX7219_PORT->BRR = (1 << MAX7219_DIN_PIN);
        
        // Clock pulse (rising edge)
        MAX7219_PORT->BSRR = (1 << MAX7219_CLK_PIN);
        delay_us(1);
        MAX7219_PORT->BRR = (1 << MAX7219_CLK_PIN);
        delay_us(1);
    }
    
    // CS high
    MAX7219_PORT->BSRR = (1 << MAX7219_CS_PIN);
}

// Inisialisasi MAX7219
void SevenSegment_Init(void) {
    GPIO_Config();
    
    // Konfigurasi MAX7219
    SevenSegment_Write(MAX7219_SCAN_LIMIT, 7);   // Scan semua 8 digit
    SevenSegment_Write(MAX7219_DECODE, 0xFF);    // Decode semua digit
    SevenSegment_Write(MAX7219_INTENSITY, 0x07); // Kecerahan medium
    SevenSegment_Write(MAX7219_SHUTDOWN, 0x01);  // Mode normal (tidak shutdown)
    SevenSegment_Clear();
}

// Tulis data ke register MAX7219
void SevenSegment_Write(uint8_t address, uint8_t data) {
    SPI_Send16(((uint16_t)address << 8) | data);
}

// Tampilkan angka (positif/negatif)
void SevenSegment_DisplayNumber(int32_t number) {
    uint8_t isNegative = 0;
    uint8_t digits[8] = {0};
    uint8_t i = 0;
    
    if(number < 0) {
        isNegative = 1;
        number = -number;
    }
    
    // Ekstrak digit
    do {
        digits[i++] = number % 10;
        number /= 10;
    } while(number > 0 && i < 8);
    
    // Tampilkan digit (dari kanan ke kiri)
    for(uint8_t pos = 0; pos < 8; pos++) {
        if(pos < i) {
            SevenSegment_Write(MAX7219_DIGIT0 + pos, digits[pos]);
        } else if(pos == i && isNegative) {
            SevenSegment_Write(MAX7219_DIGIT0 + pos, 0x40); // Tanda minus
        } else {
            SevenSegment_Write(MAX7219_DIGIT0 + pos, 0x00); // Kosong
        }
    }
}

// Tampilkan string (0-9, A-F, a-f, '-', '_')
void SevenSegment_DisplayString(const char* str) {
    uint8_t length = 0;
    const char* ptr = str;
    
    // Hitung panjang string (maks 8 karakter)
    while(*ptr && length < 8) {
        length++;
        ptr++;
    }
    
    // Tampilkan karakter (dari kanan ke kiri)
    for(uint8_t pos = 0; pos < 8; pos++) {
        if(pos < length) {
            char c = str[length - 1 - pos]; // Urutan terbalik
            
            if(c >= '0' && c <= '9') {
                SevenSegment_Write(MAX7219_DIGIT0 + pos, c - '0');
            } else if(c >= 'A' && c <= 'F') {
                SevenSegment_Write(MAX7219_DIGIT0 + pos, 0x0A + (c - 'A'));
            } else if(c >= 'a' && c <= 'f') {
                SevenSegment_Write(MAX7219_DIGIT0 + pos, 0x0A + (c - 'a'));
            } else if(c == '-') {
                SevenSegment_Write(MAX7219_DIGIT0 + pos, 0x40);
            } else if(c == '_') {
                SevenSegment_Write(MAX7219_DIGIT0 + pos, 0x08);
            } else {
                SevenSegment_Write(MAX7219_DIGIT0 + pos, 0x00); // Kosong
            }
        } else {
            SevenSegment_Write(MAX7219_DIGIT0 + pos, 0x00); // Kosong
        }
    }
}

// Atur kecerahan (0-15)
void SevenSegment_SetBrightness(uint8_t brightness) {
    if(brightness > 15) brightness = 15;
    SevenSegment_Write(MAX7219_INTENSITY, brightness);
}

// Bersihkan display
void SevenSegment_Clear(void) {
    for(uint8_t i = 0; i < 8; i++) {
        SevenSegment_Write(MAX7219_DIGIT0 + i, 0x00);
    }
}

// Mode test (semua segmen menyala)
void SevenSegment_TestMode(uint8_t enable) {
    SevenSegment_Write(MAX7219_TEST, enable ? 0x01 : 0x00);
}