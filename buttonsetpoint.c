#include "stm32f10x.h"

// Variabel untuk menyimpan nilai setpoint
static volatile int setpoint1 = 300; // Nilai default 300
static volatile int setpoint2 = 300; // Nilai default 300

// Inisialisasi pin untuk tombol
void ButtonSetpoint_Init(void) {
    // Aktifkan clock untuk GPIOA dan GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;

    // Konfigurasi pin PA7 dan PA6 untuk setpoint1 sebagai input pull-up
    GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7 | GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
    GPIOA->CRL |= (GPIO_CRL_CNF7_1 | GPIO_CRL_CNF6_1); // CNF = 10 (Input pull-up)
    GPIOA->ODR |= (GPIO_ODR_ODR7 | GPIO_ODR_ODR6);     // Pull-up enable

    // Konfigurasi pin PA5 dan PA4 untuk setpoint2 sebagai input pull-up
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5 | GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_CNF4_1); // CNF = 10 (Input pull-up)
    GPIOA->ODR |= (GPIO_ODR_ODR5 | GPIO_ODR_ODR4);     // Pull-up enable
}

// Fungsi untuk memperbarui nilai setpoint berdasarkan tombol yang ditekan
void ButtonSetpoint_Update(void) {
    // Periksa tombol untuk setpoint1
    if (!(GPIOA->IDR & GPIO_IDR_IDR7)) { // Tombol PA7 ditekan (logika pull-up)
        setpoint1 += 1; // Tambah nilai setpoint1
    }
    if (!(GPIOA->IDR & GPIO_IDR_IDR6)) { // Tombol PA6 ditekan (logika pull-up)
        setpoint1 -= 1; // Kurangi nilai setpoint1
    }

    // Periksa tombol untuk setpoint2
    if (!(GPIOA->IDR & GPIO_IDR_IDR5)) { // Tombol PA5 ditekan (logika pull-up)
        setpoint2 += 1; // Tambah nilai setpoint2
    }
    if (!(GPIOA->IDR & GPIO_IDR_IDR4)) { // Tombol PA4 ditekan (logika pull-up)
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
    return (!(GPIOA->IDR & GPIO_IDR_IDR7)) || (!(GPIOA->IDR & GPIO_IDR_IDR6)); // Logika pull-up
}

// Fungsi untuk memeriksa apakah tombol JBC ditekan
int ButtonSetpoint_JBCPressed(void) {
    return (!(GPIOA->IDR & GPIO_IDR_IDR5)) || (!(GPIOA->IDR & GPIO_IDR_IDR4)); // Logika pull-up
}
