#ifndef BUTTON_SETPOINT_H
#define BUTTON_SETPOINT_H

#include "stm32f10x.h"

// Deklarasi fungsi
void ButtonSetpoint_Init(void);
void ButtonSetpoint_Update(void);

// Fungsi untuk mendapatkan nilai setpoint
int ButtonSetpoint_GetSetpoint1(void);
int ButtonSetpoint_GetSetpoint2(void);

#endif // BUTTON_SETPOINT_H
