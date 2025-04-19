#ifndef ADC_DMA_H
#define ADC_DMA_H

#include <stdint.h>

// Inisialisasi ADC dengan DMA
void adc_dma_init(void);

// Fungsi untuk mendapatkan nilai suhu (dalam °C)
float adc_get_temperature1(void); // Sensor suhu 1 (PA0)
float adc_get_temperature2(void); // Sensor suhu 2 (PA2)

// Fungsi untuk mendapatkan nilai suhu ambient dari NTC (PA1)
float adc_get_ambient_temperature(void);

#endif // ADC_DMA_H
