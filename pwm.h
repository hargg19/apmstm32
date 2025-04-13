#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"

// Definisi channel manual jika CMSIS tidak menyediakan
typedef enum {
    PWM_CH1 = 0x00,  // TIM_Channel_1 (PA8)
    PWM_CH2 = 0x01,  // TIM_Channel_2 (PA9)
    PWM_CH3 = 0x02   // TIM_Channel_3 (PA10)
} PWM_Channel;

void PWM_Init(uint32_t freq_hz, uint16_t resolution);
void PWM_SetDuty(PWM_Channel ch, float duty);
void PWM_Enable(PWM_Channel ch);
void PWM_Disable(PWM_Channel ch);

#endif // PWM_H
