#include "buttonsetpoint.h"

// Variabel untuk menyimpan nilai setpoint
static volatile int setpoint1 = 300; // Nilai default 300
static volatile int setpoint2 = 300; // Nilai default 300

// Inisialisasi pin untuk tombol
void ButtonSetpoint_Init(void) {
    // Aktifkan clock untuk GPIOA dan GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // Konfigurasi pin PA11 dan PA12 untuk setpoint1
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // Input dengan pull-down
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Konfigurasi pin PA15 untuk setpoint2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Konfigurasi pin PB3 untuk setpoint2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// Fungsi untuk memperbarui nilai setpoint berdasarkan tombol yang ditekan
void ButtonSetpoint_Update(void) {
    // Periksa tombol untuk setpoint1
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)) {
        setpoint1 += 1; // Tambah nilai setpoint1
    }
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)) {
        setpoint1 -= 1; // Kurangi nilai setpoint1
    }

    // Periksa tombol untuk setpoint2
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)) {
        setpoint2 += 1; // Tambah nilai setpoint2
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
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
