#ifndef ITIMER_H
#define ITIMER_H

#include <stdint.h>

// Maksimum jumlah timer yang dapat dijalankan bersamaan
#define MAX_TIMERS 8

// Timer struct untuk setiap instance timer
typedef struct {
    uint8_t active;                     // Apakah timer aktif?
    uint8_t paused;                     // Apakah timer di-pause?
    uint32_t duration_ms;               // Durasi timer dalam milidetik
    uint32_t remaining_ms;              // Waktu tersisa dalam milidetik
    void (*callback)(void);             // Callback function ketika timer selesai
} TimerStruct;

// Fungsi inisialisasi TIM2 dan library itimer
void itimer_init(void);

// Fungsi untuk memulai timer
uint8_t itimer_start(uint32_t duration_ms, void (*callback)(void));

// Fungsi untuk menjeda timer
void itimer_pause(uint8_t timer_id);

// Fungsi untuk melanjutkan timer yang di-pause
void itimer_resume(uint8_t timer_id);

// Fungsi untuk menghentikan timer
void itimer_stop(uint8_t timer_id);

#endif // ITIMER_H
