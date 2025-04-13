#include "pwm.h"

static uint16_t pwm_resolution;

// Helper untuk konversi channel ke register offset
#define CH_TO_CCR(ch) (*(&TIM1->CCR1 + (ch)))

void PWM_Init(uint32_t freq_hz, uint16_t resolution) {
    pwm_resolution = resolution;
    
    /* 1. Enable Clock */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
    
    /* 2. Configure GPIO Pins */
    GPIOA->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9 | GPIO_CRH_CNF10);
    GPIOA->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_1); // AFPP
    GPIOA->CRH |= (GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1 | 
                   GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 |
                   GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1); // 50MHz
    
    /* 3. Timer Base Configuration */
    uint16_t prescaler = (SystemCoreClock / (resolution * freq_hz)) - 1;
    TIM1->PSC = prescaler;
    TIM1->ARR = resolution - 1;
    TIM1->CR1 = TIM_CR1_ARPE;
    
    /* 4. Configure Output Compare Mode */
    // Channel 1-3 PWM Mode 1 (110)
    TIM1->CCMR1 = (6 << 4) | (6 << 12); // OC1M dan OC2M
    TIM1->CCMR2 = (6 << 4);              // OC3M
    
    /* 5. Enable Preload */
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
    TIM1->CCMR2 |= TIM_CCMR2_OC3PE;
    
    /* 6. Enable Outputs */
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E;
    TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->CR1 |= TIM_CR1_CEN;
}

void PWM_SetDuty(PWM_Channel ch, float duty) {
    duty = (duty < 0.0f) ? 0.0f : (duty > 100.0f) ? 100.0f : duty;
    CH_TO_CCR(ch) = (uint16_t)((duty / 100.0f) * pwm_resolution);
}

void PWM_Enable(PWM_Channel ch) {
    TIM1->CCER |= (TIM_CCER_CC1E << ch);
}

void PWM_Disable(PWM_Channel ch) {
    TIM1->CCER &= ~(TIM_CCER_CC1E << ch);
}
