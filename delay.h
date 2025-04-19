#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

// Inisialisasi SysTick untuk delay
void delay_init(void);

// Delay dalam milidetik
void delay_ms(uint32_t ms);

// Delay dalam mikrodetik
void delay_us(uint32_t us);

// Fungsi untuk mendapatkan waktu berjalan dalam milidetik
uint32_t delay_millis(void);

#endif // DELAY_H
