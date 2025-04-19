#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

// Inisialisasi SysTick untuk delay
void delay_init(void);

// Delay dalam milidetik
void delay_ms(uint32_t ms);

// Delay dalam mikrodetik
void delay_us(uint32_t us);

#endif // DELAY_H
