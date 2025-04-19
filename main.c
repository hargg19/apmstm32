#include "adc_dma.h"
#include "pid_v1.h"
#include "7display.h"
#include "itimer.h"
#include "pwm.h"
#include "buttonsetpoint.h"
#include "onofftrigger.h"

// Deklarasi PID Controller untuk T12 dan JBC
PID_Controller pid_T12, pid_JBC;

// Variabel global
volatile int show_setpoint = 0; // Flag untuk menampilkan setpoint (0 = suhu, 1 = setpoint)
volatile int setpoint_T12 = 300, setpoint_JBC = 300; // Nilai setpoint default
volatile int heater_on_T12 = 0, heater_on_JBC = 0; // Status on/off heater

// Fungsi untuk mengupdate display
void update_display(void) {
    if (show_setpoint) {
        // Tampilkan setpoint
        MAX7219_DisplayChar(0, setpoint_T12 / 100 % 10); // Ratusan
        MAX7219_DisplayChar(1, setpoint_T12 / 10 % 10);  // Puluhan
        MAX7219_DisplayChar(2, setpoint_T12 % 10);       // Satuan
    } else {
        // Tampilkan suhu aktual
        float temp_T12 = adc_get_temperature1();
        MAX7219_DisplayChar(0, (int)temp_T12 / 100 % 10); // Ratusan
        MAX7219_DisplayChar(1, (int)temp_T12 / 10 % 10);  // Puluhan
        MAX7219_DisplayChar(2, (int)temp_T12 % 10);       // Satuan
    }
}

// Fungsi kontrol suhu dengan PID
void control_heater(void) {
    float temp_T12 = adc_get_temperature1();
    float temp_JBC = adc_get_temperature2();

    // Hitung output PID
    float output_T12 = PID_Compute(&pid_T12, temp_T12, setpoint_T12);
    float output_JBC = PID_Compute(&pid_JBC, temp_JBC, setpoint_JBC);

    // Kontrol heater dengan PWM berdasarkan status on/off
    if (heater_on_T12) {
        Gmos1_SetDuty((uint8_t)output_T12);
    } else {
        Gmos1_SetDuty(0);
    }

    if (heater_on_JBC) {
        Gmos2_SetDuty((uint8_t)output_JBC);
    } else {
        Gmos2_SetDuty(0);
    }
}

// Callback ITimer setiap 200ms
void timer_callback(void) {
    // Perbarui display
    update_display();

    // Kontrol heater
    control_heater();
}

// Callback untuk tombol setpoint
void button_callback(void) {
    // Pause ITimer
    itimer_pause(0);

    // Tampilkan setpoint selama 2 detik
    show_setpoint = 1;
    update_display();
    delay_ms(2000);

    // Kembali ke suhu
    show_setpoint = 0;

    // Resume ITimer
    itimer_resume(0);
}

// Fungsi utama
int main(void) {
    // Inisialisasi hardware
    adc_dma_init();
    MAX7219_Init();
    PWM_Init();
    ButtonSetpoint_Init();
    OnOffTrigger_Init();
    itimer_init();

    // Inisialisasi PID
    PID_Init(&pid_T12, 2.0, 0.5, 0.1, 0, 255);
    PID_Init(&pid_JBC, 2.0, 0.5, 0.1, 0, 255);

    // Mulai ITimer dengan interval 200ms
    itimer_start(200, timer_callback);

    // Loop utama
    while (1) {
        // Perbarui setpoint jika tombol ditekan
        ButtonSetpoint_Update();
        setpoint_T12 = ButtonSetpoint_GetSetpoint1();
        setpoint_JBC = ButtonSetpoint_GetSetpoint2();

        // Periksa status on/off heater
        heater_on_T12 = OnOffTrigger_GetStatus1();
        heater_on_JBC = OnOffTrigger_GetStatus2();
    }
}
