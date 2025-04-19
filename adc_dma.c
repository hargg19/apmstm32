#include "stm32f10x.h"
#include <math.h>
#include <string.h>

#define SMA_SAMPLE_SIZE 16  // Jumlah sample untuk averaging

// Buffer ADC untuk DMA (PA0, PA1, dan PA2)
volatile uint16_t adc_buffer[3]; // PA0 (Temp1), PA1 (Temp2), dan PA2 (CJC/Ambient)

// Struktur untuk SMA
typedef struct {
    uint16_t buffer[SMA_SAMPLE_SIZE];
    uint8_t index;
    uint32_t sum;
    uint8_t is_ready;
} sma_filter_t;

static sma_filter_t sma[3];  // Untuk PA0, PA1, PA2

// Konstanta untuk NTC
#define SERIES_RESISTOR 10000.0f
#define BETA_COEFFICIENT 3950.0f
#define ROOM_TEMP 298.15f
#define NTC_REF_RESISTANCE 10000.0f
#define OFFSET 1.65f

// Inisialisasi ADC dan DMA
void adc_dma_init(void) {
    // Aktifkan clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // Konfigurasi pin analog
    GPIOA->CRL &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)) | (0xF << (2 * 4)));

    // Konfigurasi DMA
    DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
    DMA1_Channel1->CMAR = (uint32_t)adc_buffer;
    DMA1_Channel1->CNDTR = 3;
    DMA1_Channel1->CCR = DMA_CCR1_MINC | DMA_CCR1_PSIZE_0 | DMA_CCR1_MSIZE_0 |
                         DMA_CCR1_CIRC | DMA_CCR1_PL_1 | DMA_CCR1_EN;

    // Konfigurasi ADC
    ADC1->CR1 = ADC_CR1_SCAN;
    ADC1->CR2 = ADC_CR2_DMA | ADC_CR2_CONT | ADC_CR2_ADON;

    // Sampling time
    ADC1->SMPR2 = (ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP1_2 | ADC_SMPR2_SMP2_2);

    // Urutan konversi
    ADC1->SQR1 = (3 - 1) << 20;
    ADC1->SQR3 = (0 << 0) | (1 << 5) | (2 << 10);

    // Kalibrasi ADC
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while (ADC1->CR2 & ADC_CR2_RSTCAL);
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);

    // Aktifkan ADC
    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Inisialisasi SMA
    memset(&sma, 0, sizeof(sma));
}

// Update SMA secara internal
static void update_sma(void) {
    for (int i = 0; i < 3; i++) {
        if (sma[i].is_ready) {
            sma[i].sum -= sma[i].buffer[sma[i].index];
        } else if (sma[i].index == SMA_SAMPLE_SIZE-1) {
            sma[i].is_ready = 1;
        }
        
        sma[i].buffer[sma[i].index] = adc_buffer[i];
        sma[i].sum += adc_buffer[i];
        sma[i].index = (sma[i].index + 1) % SMA_SAMPLE_SIZE;
    }
}

// Fungsi pembacaan dengan SMA internal
float adc_get_cjc_temperature(void) {
    update_sma();
    
    uint16_t avg_adc = sma[2].sum / (sma[2].is_ready ? SMA_SAMPLE_SIZE : sma[2].index);
    float Vout = (avg_adc * 3.3f) / 4095.0f;
    float R_ntc = 10000.0f * (3.3f / Vout - 1.0f);
    float temp_kelvin = 1.0f / (1.0f / 298.15f + 1.0f / 3950.0f * logf(R_ntc / 10000.0f));
    return temp_kelvin - 273.15f;
}

float adc_get_temperature1(void) {
    update_sma();
    
    uint16_t avg_adc = sma[0].sum / (sma[0].is_ready ? SMA_SAMPLE_SIZE : sma[0].index);
    float temp1 = (avg_adc * 500.0f) / 4095.0f;
    float cjc_temp = adc_get_cjc_temperature();
    return temp1 + cjc_temp;
}

float adc_get_temperature2(void) {
    update_sma();
    
    uint16_t avg_adc = sma[1].sum / (sma[1].is_ready ? SMA_SAMPLE_SIZE : sma[1].index);
    float temp2 = (avg_adc * 500.0f) / 4095.0f;
    float cjc_temp = adc_get_cjc_temperature();
    return temp2 + cjc_temp;
}
