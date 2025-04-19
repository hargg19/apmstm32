#include "stm32f10x.h"

// Variabel untuk menyimpan nilai setpoint
static volatile int setpoint1 = 300; // Nilai default 300
static volatile int setpoint2 = 300; // Nilai default 300

// Inisialisasi pin untuk tombol
void ButtonSetpoint_Init(void) {
    // Aktifkan clock untuk GPIOA dan GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;

    // Konfigurasi pin PA11 dan PA12 untuk setpoint1 sebagai input pull-down
    GPIOA->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11 | GPIO_CRH_MODE12 | GPIO_CRH_CNF12);
    GPIOA->CRH |= (GPIO_CRH_CNF11_1 | GPIO_CRH_CNF12_1); // CNF = 10 (Input pull-down)
    GPIOA->ODR &= ~(GPIO_ODR_ODR11 | GPIO_ODR_ODR12);    // Pull-down enable

    // Konfigurasi pin PA15 untuk setpoint2 sebagai input pull-down
    GPIOA->CRH &= ~(GPIO_CRH_MODE15 | GPIO_CRH_CNF15);
    GPIOA->CRH |= GPIO_CRH_CNF15_1; // CNF = 10 (Input pull-down)
    GPIOA->ODR &= ~GPIO_ODR_ODR15;  // Pull-down enable

    // Konfigurasi pin PB3 untuk setpoint2 sebagai input pull-down
    GPIOB->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
    GPIOB->CRL |= GPIO_CRL_CNF3_1; // CNF = 10 (Input pull-down)
    GPIOB->ODR &= ~GPIO_ODR_ODR3;  // Pull-down enable
}

// Fungsi untuk memperbarui nilai setpoint berdasarkan tombol yang ditekan
void ButtonSetpoint_Update(void) {
    // Periksa tombol untuk setpoint1
    if (GPIOA->IDR & GPIO_IDR_IDR11) {
        setpoint1 += 1; // Tambah nilai setpoint1
    }
    if (GPIOA->IDR & GPIO_IDR_IDR12) {
        setpoint1 -= 1; // Kurangi nilai setpoint1
    }

    // Periksa tombol untuk setpoint2
    if (GPIOA->IDR & GPIO_IDR_IDR15) {
        setpoint2 += 1; // Tambah nilai setpoint2
    }
    if (GPIOB->IDR & GPIO_IDR_IDR3) {
        setpoint2 -= 1; // Kurangi nilai setpoint2
    }
}

// Fungsi untuk mendapatkan nilai setpoint1
int ButtonSetpoint_GetSetpoint1(void) {
    return setpoint1;
}

// Fungsi untuk mendapatkan nilai setpoint2
int ButtonSetpoint_GetSetpoint2(void) {
    return setpoint2;
}

// Fungsi untuk memeriksa apakah tombol T12 ditekan
int ButtonSetpoint_T12Pressed(void) {
    return (GPIOA->IDR & GPIO_IDR_IDR11) || (GPIOA->IDR & GPIO_IDR_IDR12);
}

// Fungsi untuk memeriksa apakah tombol JBC ditekan
int ButtonSetpoint_JBCPressed(void) {
    return (GPIOA->IDR & GPIO_IDR_IDR15) || (GPIOB->IDR & GPIO_IDR_IDR3);
}
