#include "pwm.h"
#include "stm32f10x.h"

void PWM_Init(void) {
    // Aktifkan clock untuk GPIOB dan TIM1
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;  // Enable GPIOB clock
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Enable TIM1 clock

    // Konfigurasi PB8 dan PB9 sebagai Alternate Function Push-Pull
    GPIOB->CRH &= ~((0xF << (8 - 8) * 4) | (0xF << (9 - 8) * 4)); // Clear konfigurasi pin PB8 dan PB9
    GPIOB->CRH |= (0xB << (8 - 8) * 4); // PB8 sebagai AF Push-Pull
    GPIOB->CRH |= (0xB << (9 - 8) * 4); // PB9 sebagai AF Push-Pull

    // Konfigurasi Timer 1 untuk PWM
    TIM1->PSC = 71;                     // Prescaler: Timer clock = 72 MHz / (71 + 1) = 1 MHz
    TIM1->ARR = 499;                    // Auto-reload register: Default untuk 2 kHz (1 MHz / (499 + 1))
    TIM1->CR1 |= TIM_CR1_ARPE;          // Auto-reload preload enable
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // PWM mode 1 untuk CH1 (PB8)
    TIM1->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; // PWM mode 1 untuk CH2 (PB9)
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;   // Enable preload untuk CH1 dan CH2
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;        // Enable output untuk CH1 dan CH2
    TIM1->BDTR |= TIM_BDTR_MOE;         // Main output enable
    TIM1->CR1 |= TIM_CR1_CEN;           // Enable counter
}

void Gmos1_SetDuty(uint8_t duty) {
    if (duty > 80) duty = 80; // Duty cycle maksimum 80%
    TIM1->CCR1 = (TIM1->ARR + 1) * duty / 100; // Set nilai CCR1 (PB8)
}

void Gmos2_SetDuty(uint8_t duty) {
    if (duty > 90) duty = 90; // Duty cycle maksimum 90%
    TIM1->CCR2 = (TIM1->ARR + 1) * duty / 100; // Set nilai CCR2 (PB9)
}

void Gmos1_SetFreq(uint32_t freq) {
    uint32_t period = (1000000 / freq) - 1; // Timer clock = 1 MHz
    TIM1->ARR = period; // Set auto-reload register
}

void Gmos2_SetFreq(uint32_t freq) {
    uint32_t period = (1000000 / freq) - 1; // Timer clock = 1 MHz
    TIM1->ARR = period; // Set auto-reload register
}
