#ifndef ADC_DMA_H
#define ADC_DMA_H

#include <stdint.h>
#include "stm32f10x.h"

// Buffer untuk menyimpan hasil ADC (2 channel: PA0 & PA1)
#define ADC_BUFFER_SIZE  2
extern volatile uint16_t adc_buffer[ADC_BUFFER_SIZE];

// Fungsi Inisialisasi
void ADC_DMA_Init(void);
uint16_t ADC_Read_PA0(void);
uint16_t ADC_Read_PA1(void);

#endif // ADC_DMA_H