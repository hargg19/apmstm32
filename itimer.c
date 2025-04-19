#include "itimer.h"
#include "stm32f10x.h"
#include <stdlib.h>

// Array untuk menyimpan semua timer
static TimerStruct timers[MAX_TIMERS];

// Inisialisasi TIM2 untuk interrupt berbasis waktu
void itimer_init(void) {
    // Aktifkan clock untuk TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Atur prescaler untuk menghasilkan tick 1 ms
    TIM2->PSC = (SystemCoreClock / 1000) - 1; // Prescaler untuk 1 ms tick
    TIM2->ARR = 10;                            // Auto-reload register (1 tick = 1 ms)

    // Aktifkan interrupt TIM2
    TIM2->DIER |= TIM_DIER_UIE;

    // Mulai timer
    TIM2->CR1 |= TIM_CR1_CEN;

    // Konfigurasi NVIC untuk TIM2 interrupt
    NVIC_EnableIRQ(TIM2_IRQn);

    // Inisialisasi semua timer sebagai tidak aktif
    for (uint8_t i = 0; i < MAX_TIMERS; i++) {
        timers[i].active = 0;
        timers[i].paused = 0;
        timers[i].remaining_ms = 0;
        timers[i].callback = NULL;
    }
}

// Fungsi untuk memulai timer
uint8_t itimer_start(uint32_t duration_ms, void (*callback)(void)) {
    for (uint8_t i = 0; i < MAX_TIMERS; i++) {
        if (!timers[i].active) {
            timers[i].active = 1;
            timers[i].paused = 0;
            timers[i].duration_ms = duration_ms;
            timers[i].remaining_ms = duration_ms;
            timers[i].callback = callback;
            return i; // Kembalikan ID timer
        }
    }
    return 255; // Tidak ada slot timer yang tersedia
}

// Fungsi untuk menjeda timer
void itimer_pause(uint8_t timer_id) {
    if (timer_id < MAX_TIMERS && timers[timer_id].active && !timers[timer_id].paused) {
        timers[timer_id].paused = 1;
    }
}

// Fungsi untuk melanjutkan timer yang di-pause
void itimer_resume(uint8_t timer_id) {
    if (timer_id < MAX_TIMERS && timers[timer_id].active && timers[timer_id].paused) {
        timers[timer_id].paused = 0;
    }
}

// Fungsi untuk menghentikan timer
void itimer_stop(uint8_t timer_id) {
    if (timer_id < MAX_TIMERS) {
        timers[timer_id].active = 0;
        timers[timer_id].paused = 0;
        timers[timer_id].remaining_ms = 0;
        timers[timer_id].callback = NULL;
    }
}

// Interrupt Service Routine untuk TIM2
void TIM2_IRQHandler(void) {
    // Pastikan interrupt berasal dari update event
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF; // Clear interrupt flag

        // Periksa semua timer
        for (uint8_t i = 0; i < MAX_TIMERS; i++) {
            if (timers[i].active && !timers[i].paused) {
                if (timers[i].remaining_ms > 0) {
                    timers[i].remaining_ms--; // Kurangi waktu tersisa
                }
                if (timers[i].remaining_ms == 0) {
                    timers[i].active = 0; // Matikan timer
                    if (timers[i].callback) {
                        timers[i].callback(); // Panggil callback jika ada
                    }
                }
            }
        }
    }
}
