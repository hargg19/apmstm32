#include "pwm.h"

#define PWM_FREQ     50000  // 50kHz
#define PWM_RES      1000   // 0-1000 = 0-100% resolution

void PWM_Init(void) {
    // 1. Enable Clocks
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
    
    // 2. Configure GPIO (PA8/PA9/PA10 - Alternate Function Push-Pull)
    GPIOA->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9 | GPIO_CRH_CNF10);
    GPIOA->CRH |= (0xB << GPIO_CRH_CNF8_Pos) |  // CH1 (PA8)
                  (0xB << GPIO_CRH_CNF9_Pos) |  // CH2 (PA9)
                  (0xB << GPIO_CRH_CNF10_Pos);  // CH3 (PA10)
    GPIOA->CRH |= (0x3 << GPIO_CRH_MODE8_Pos) | // 50MHz
                  (0x3 << GPIO_CRH_MODE9_Pos) |
                  (0x3 << GPIO_CRH_MODE10_Pos);
    
    // 3. Timer Base Configuration
    uint16_t prescaler = (SystemCoreClock / (PWM_RES * PWM_FREQ)) - 1;
    TIM1->PSC = prescaler;
    TIM1->ARR = PWM_RES - 1;
    TIM1->CR1 = TIM_CR1_ARPE; // Auto-reload preload enabled
    
    // 4. Output Compare Configuration
    TIM1->CCMR1 = (6 << TIM_CCMR1_OC1M_Pos) | // PWM mode 1
                  (6 << TIM_CCMR1_OC2M_Pos);
    TIM1->CCMR2 = (6 << TIM_CCMR2_OC3M_Pos);
    
    TIM1->CCER |= TIM_CCER_CC1E | // Enable CH1
                  TIM_CCER_CC2E | // Enable CH2
                  TIM_CCER_CC3E;  // Enable CH3
    
    // 5. Enable PWM Outputs
    TIM1->BDTR |= TIM_BDTR_MOE; // Main output enable
    TIM1->CR1 |= TIM_CR1_CEN;   // Counter enable
}

void PWM_SetDuty(PWM_Channel ch, uint16_t duty) {
    if(duty > PWM_RES) duty = PWM_RES;
    
    switch(ch) {
        case PWM_HEATER_T12:
            TIM1->CCR1 = duty;
            break;
        case PWM_HEATER_HAIR:
            TIM1->CCR2 = duty;
            break;
        case PWM_FAN:
            TIM1->CCR3 = duty;
            break;
    }
}

void PWM_Enable(PWM_Channel ch) {
    switch(ch) {
        case PWM_HEATER_T12:
            TIM1->CCER |= TIM_CCER_CC1E;
            break;
        case PWM_HEATER_HAIR:
            TIM1->CCER |= TIM_CCER_CC2E;
            break;
        case PWM_FAN:
            TIM1->CCER |= TIM_CCER_CC3E;
            break;
    }
}

void PWM_Disable(PWM_Channel ch) {
    switch(ch) {
        case PWM_HEATER_T12:
            TIM1->CCER &= ~TIM_CCER_CC1E;
            break;
        case PWM_HEATER_HAIR:
            TIM1->CCER &= ~TIM_CCER_CC2E;
            break;
        case PWM_FAN:
            TIM1->CCER &= ~TIM_CCER_CC3E;
            break;
    }
}
