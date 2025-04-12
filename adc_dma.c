#include "adc_dma.h"

// Buffer untuk DMA (PA0 & PA1)
volatile uint16_t adc_buffer[ADC_BUFFER_SIZE] = {0};

void ADC_DMA_Init(void) {
    // 1. Enable Clock untuk GPIOA, ADC1, dan DMA1
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC1EN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. Konfigurasi PA0 & PA1 sebagai Analog Input
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_MODE0 | GPIO_CRL_MODE1);

    // 3. Konfigurasi ADC1
    ADC1->CR2 = 0;
    ADC1->CR2 |= ADC_CR2_CONT;   // Mode kontinu
    ADC1->CR2 |= ADC_CR2_DMA;    // Aktifkan DMA
    ADC1->CR2 |= ADC_CR2_ADON;   // Enable ADC

    // 4. Konfigurasi DMA1 Channel 1 (untuk ADC1)
    DMA1_Channel1->CCR = 0;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;    // Memory increment
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;    // Circular mode
    DMA1_Channel1->CCR |= DMA_CCR1_TCIE;    // Transfer Complete Interrupt Enable
    DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;   // Peripheral address (ADC Data Register)
    DMA1_Channel1->CMAR = (uint32_t)adc_buffer;  // Memory address (buffer)
    DMA1_Channel1->CNDTR = ADC_BUFFER_SIZE;      // Jumlah data

    // 5. Enable DMA Channel
    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    // 6. Konfigurasi Urutan ADC (PA0 -> PA1)
    ADC1->SQR1 = 0;  // 1 konversi per sequence
    ADC1->SQR3 = (0 << 0) | (1 << 5);  // Urutan: Channel 0 (PA0), lalu Channel 1 (PA1)

    // 7. Kalibrasi ADC
    ADC1->CR2 |= ADC_CR2_RSTCAL;  // Reset kalibrasi
    while(ADC1->CR2 & ADC_CR2_RSTCAL);
    ADC1->CR2 |= ADC_CR2_CAL;     // Mulai kalibrasi
    while(ADC1->CR2 & ADC_CR2_CAL);

    // 8. Mulai konversi ADC
    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_SWSTART;  // Mulai konversi pertama
}

// Baca nilai PA0 (ADC Channel 0)
uint16_t ADC_Read_PA0(void) {
    return adc_buffer[0];
}

// Baca nilai PA1 (ADC Channel 1)
uint16_t ADC_Read_PA1(void) {
    return adc_buffer[1];
}