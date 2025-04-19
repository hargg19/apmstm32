#include "stm32f10x.h"

// Variabel untuk status on/off
static volatile uint8_t onoff1 = 0; // Default off
static volatile uint8_t onoff2 = 0; // Default off

// Fungsi inisialisasi pin PB10 dan PB11 sebagai interrupt
void OnOffTrigger_Init(void) {
    // Aktifkan clock untuk GPIOB dan AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

    // Konfigurasi pin PB10 dan PB11 sebagai input pull-up
    GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_MODE11); // Set MODE = 00 (Input)
    GPIOB->CRH |= GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1;  // Set CNF = 10 (Input pull-up)
    GPIOB->ODR |= GPIO_ODR_ODR10 | GPIO_ODR_ODR11;      // Pull-up enable

    // Konfigurasi interrupt eksternal untuk PB10 dan PB11
    AFIO->EXTICR[2] &= ~(AFIO_EXTICR3_EXTI10 | AFIO_EXTICR3_EXTI11);
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PB | AFIO_EXTICR3_EXTI11_PB;

    // Atur EXTI untuk PB10
    EXTI->IMR |= EXTI_IMR_MR10;         // Unmask EXTI Line 10
    EXTI->EMR &= ~EXTI_EMR_MR10;        // Mask event
    EXTI->RTSR |= EXTI_RTSR_TR10;       // Trigger rising edge
    EXTI->FTSR |= EXTI_FTSR_TR10;       // Trigger falling edge

    // Atur EXTI untuk PB11
    EXTI->IMR |= EXTI_IMR_MR11;         // Unmask EXTI Line 11
    EXTI->EMR &= ~EXTI_EMR_MR11;        // Mask event
    EXTI->RTSR |= EXTI_RTSR_TR11;       // Trigger rising edge
    EXTI->FTSR |= EXTI_FTSR_TR11;       // Trigger falling edge

    // Konfigurasi NVIC untuk EXTI Line 10-15
    NVIC_SetPriority(EXTI15_10_IRQn, 0); // Prioritas interrupt
    NVIC_EnableIRQ(EXTI15_10_IRQn);      // Aktifkan interrupt
}

// Fungsi interrupt handler untuk EXTI line 10-15
void EXTI15_10_IRQHandler(void) {
    // Periksa apakah interrupt terjadi pada PB10
    if (EXTI->PR & EXTI_PR_PR10) {
        // Toggle status onoff1 berdasarkan level pin
        if (GPIOB->IDR & GPIO_IDR_IDR10) {
            onoff1 = 1; // High = ON
        } else {
            onoff1 = 0; // Low = OFF
        }
        // Clear interrupt flag
        EXTI->PR = EXTI_PR_PR10;
    }

    // Periksa apakah interrupt terjadi pada PB11
    if (EXTI->PR & EXTI_PR_PR11) {
        // Toggle status onoff2 berdasarkan level pin
        if (GPIOB->IDR & GPIO_IDR_IDR11) {
            onoff2 = 1; // High = ON
        } else {
            onoff2 = 0; // Low = OFF
        }
        // Clear interrupt flag
        EXTI->PR = EXTI_PR_PR11;
    }
}

// Fungsi untuk mendapatkan status onoff1
uint8_t OnOffTrigger_GetStatus1(void) {
    return onoff1;
}

// Fungsi untuk mendapatkan status onoff2
uint8_t OnOffTrigger_GetStatus2(void) {
    return onoff2;
}
