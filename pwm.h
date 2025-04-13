#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"

// Channel Definitions
typedef enum {
    PWM_HEATER_T12 = 0,  // PA8 - TIM1_CH1
    PWM_HEATER_HAIR,     // PA9 - TIM1_CH2
    PWM_FAN             // PA10 - TIM1_CH3
} PWM_Channel;

void PWM_Init(void);
void PWM_SetDuty(PWM_Channel ch, uint16_t duty); // duty: 0-1000 (0-100%)
void PWM_Enable(PWM_Channel ch);
void PWM_Disable(PWM_Channel ch);

#endif // PWM_H
