#ifndef PID_V1_H
#define PID_V1_H

#include <stdint.h>

typedef struct {
    float Kp;               // Proportional gain
    float Ki;               // Integral gain
    float Kd;               // Derivative gain
    float setpoint;         // Target value
    float input;            // Current input value
    float output;           // Computed output
    float output_min;       // Minimum output limit
    float output_max;       // Maximum output limit
    float integral_term;    // Accumulated integral term
    float last_input;       // Last input for derivative calculation
    uint32_t last_time;     // Last computation time (ms)
} PID_Controller;

// Initialize PID controller
void PID_Init(
    PID_Controller* pid,
    float Kp, float Ki, float Kd,
    float output_min, float output_max
);

// Compute PID output
float PID_Compute(PID_Controller* pid, float input, float setpoint);

// Reset PID internal terms (integral, derivative)
void PID_Reset(PID_Controller* pid);

// Set tuning parameters
void PID_SetTunings(PID_Controller* pid, float Kp, float Ki, float Kd);

// Set output limits
void PID_SetOutputLimits(PID_Controller* pid, float min, float max);

#endif // PID_V1_H