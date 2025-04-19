#ifndef __PWM_H
#define __PWM_H

#include <stdint.h>

void PWM_Init(void);
void Gmos1_SetDuty(uint8_t duty);
void Gmos2_SetDuty(uint8_t duty);
void Gmos1_SetFreq(uint32_t freq);
void Gmos2_SetFreq(uint32_t freq);

#endif // __PWM_H
