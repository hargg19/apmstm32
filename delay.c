#include "delay.h"
#include "stm32f10x.h"

// Variabel untuk mencatat waktu berjalan dalam milidetik
static volatile uint32_t millis = 0;

// Fungsi handler SysTick
void SysTick_Handler(void) {
    millis++; // Increment setiap 1 ms
}

// Inisialisasi SysTick untuk delay dan waktu berjalan
void delay_init(void) {
    // Konfigurasi SysTick untuk menghasilkan interrupt setiap 1 ms
    SysTick_Config(SystemCoreClock / 1000); // 1 ms tick

    // Pastikan SysTick interrupt diaktifkan
    NVIC_SetPriority(SysTick_IRQn, 0); // Prioritas tertinggi
    NVIC_EnableIRQ(SysTick_IRQn);
}

// Fungsi untuk mendapatkan waktu berjalan dalam milidetik
uint32_t delay_millis(void) {
    return millis; // Kembalikan nilai waktu berjalan
}

// Fungsi delay dalam mikrodetik
void delay_us(uint32_t us) {
    uint32_t start_tick = SysTick->VAL; // Ambil nilai awal dari SysTick
    uint32_t ticks = us * (SystemCoreClock / 1000000); // Hitung jumlah tick untuk us

    // Tunggu hingga delay selesai
    while ((start_tick - SysTick->VAL) < ticks) {
        if (SysTick->VAL > start_tick) { // Jika counter SysTick melingkar
            ticks -= start_tick + (0xFFFFFF - SysTick->VAL);
            start_tick = 0xFFFFFF;
        }
    }
}

// Fungsi delay dalam milidetik
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        delay_us(1000); // 1 ms = 1000 us
    }
}
