#include "pid_v1.h"
#include "delay.h"  // Untuk fungsi waktu (ms)

void PID_Init(
    PID_Controller* pid,
    float Kp, float Ki, float Kd,
    float output_min, float output_max
) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->output_min = output_min;
    pid->output_max = output_max;
    pid->integral_term = 0;
    pid->last_input = 0;
    pid->last_time = delay_millis();  // Fungsi dari delay.h
}

float PID_Compute(PID_Controller* pid, float input, float setpoint) {
    // Hitung waktu sejak komputasi terakhir (dalam detik)
    uint32_t now = delay_millis();
    float delta_time = (now - pid->last_time) / 1000.0f;
    pid->last_time = now;

    // Hitung error
    float error = setpoint - input;

    // Term Proporsional
    float proportional = pid->Kp * error;

    // Term Integral (anti-windup clamping)
    pid->integral_term += pid->Ki * error * delta_time;
    
    // Clamp integral term
    if (pid->integral_term > pid->output_max)
        pid->integral_term = pid->output_max;
    else if (pid->integral_term < pid->output_min)
        pid->integral_term = pid->output_min;

    // Term Derivatif (filtered)
    float derivative = pid->Kd * (input - pid->last_input) / delta_time;
    pid->last_input = input;

    // Hitung output
    pid->output = proportional + pid->integral_term - derivative;

    // Clamp output
    if (pid->output > pid->output_max)
        pid->output = pid->output_max;
    else if (pid->output < pid->output_min)
        pid->output = pid->output_min;

    return pid->output;
}

void PID_Reset(PID_Controller* pid) {
    pid->integral_term = 0;
    pid->last_input = 0;
    pid->last_time = delay_millis();
}

void PID_SetTunings(PID_Controller* pid, float Kp, float Ki, float Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}

void PID_SetOutputLimits(PID_Controller* pid, float min, float max) {
    pid->output_min = min;
    pid->output_max = max;
}