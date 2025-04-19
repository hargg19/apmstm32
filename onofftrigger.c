#include "onofftrigger.h"

// Variabel untuk status on/off
static volatile uint8_t onoff1 = 0; // Default off
static volatile uint8_t onoff2 = 0; // Default off

// Fungsi inisialisasi pin PB10 dan PB11 sebagai interrupt
void OnOffTrigger_Init(void) {
    // Aktifkan clock untuk GPIOB dan AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // Konfigurasi pin PB10 dan PB11 sebagai input pull-up
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Input dengan pull-up internal
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Konfigurasi interrupt eksternal untuk PB10 dan PB11
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10); // PB10
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11); // PB11

    // Atur EXTI untuk PB10
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // Both edges
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Atur EXTI untuk PB11
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_Init(&EXTI_InitStructure);

    // Aktifkan interrupt di NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// Fungsi interrupt handler untuk EXTI line 10-15
void EXTI15_10_IRQHandler(void) {
    // Periksa apakah interrupt terjadi pada PB10
    if (EXTI_GetITStatus(EXTI_Line10) != RESET) {
        // Toggle status onoff1 berdasarkan level pin
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)) {
            onoff1 = 1; // High = ON
        } else {
            onoff1 = 0; // Low = OFF
        }
        // Clear interrupt flag
        EXTI_ClearITPendingBit(EXTI_Line10);
    }

    // Periksa apakah interrupt terjadi pada PB11
    if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
        // Toggle status onoff2 berdasarkan level pin
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)) {
            onoff2 = 1; // High = ON
        } else {
            onoff2 = 0; // Low = OFF
        }
        // Clear interrupt flag
        EXTI_ClearITPendingBit(EXTI_Line11);
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
