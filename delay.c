#include "delay.h"
#include "stm32f10x.h"

// Variabel untuk menghitung nilai tick
static uint32_t us_ticks;

// Inisialisasi SysTick untuk delay
void delay_init(void) {
    // Konfigurasi SysTick untuk menghasilkan tick setiap 1 us
    us_ticks = SystemCoreClock / 1000000;

    // Atur SysTick untuk interval maksimum
    SysTick->LOAD = 0xFFFFFF; // Nilai maksimum
    SysTick->VAL = 0;         // Reset nilai counter
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; // Aktifkan SysTick tanpa interrupt
}

// Delay dalam mikrodetik
void delay_us(uint32_t us) {
    uint32_t start_tick = SysTick->VAL; // Ambil nilai awal dari SysTick
    uint32_t ticks = us * us_ticks;

    // Tunggu hingga delay selesai
    while ((start_tick - SysTick->VAL) < ticks) {
        if (SysTick->VAL > start_tick) { // Jika counter SysTick melingkar
            ticks -= start_tick + (0xFFFFFF - SysTick->VAL);
            start_tick = 0xFFFFFF;
        }
    }
}

// Delay dalam milidetik
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        delay_us(1000); // 1 ms = 1000 us
    }
}
