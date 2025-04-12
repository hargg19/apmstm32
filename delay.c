#include "delay.h"

static volatile uint32_t overflow_count = 0;

void TIM5_IRQHandler(void)
{
    if(TIM5->SR & TIM_SR_UIF)
    {
        TIM5->SR &= ~TIM_SR_UIF;
        overflow_count++;
    }
}

void delay_init(void)
{
    // 1. Enable clock for TIM5
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    
    // 2. Configure timer for 1MHz (1us resolution)
    // Assuming 72MHz APB1 clock (default)
    TIM5->PSC = (SystemCoreClock / 1000000) - 1;
    
    // 3. Set auto-reload to maximum (16-bit)
    TIM5->ARR = 0xFFFF;
    
    // 4. Enable overflow interrupt
    TIM5->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM5_IRQn);
    NVIC_SetPriority(TIM5_IRQn, 0);
    
    // 5. Start timer
    TIM5->CR1 |= TIM_CR1_CEN;
}

void delay_us(uint32_t us)
{
    uint32_t start = delay_micros();
    while((delay_micros() - start) < us);
}

void delay_ms(uint32_t ms)
{
    uint32_t start = delay_millis();
    while((delay_millis() - start) < ms);
}

uint32_t delay_micros(void)
{
    uint32_t cnt, ovf;
    
    do {
        ovf = overflow_count;
        cnt = TIM5->CNT;
    } while(ovf != overflow_count); // Handle race condition
    
    return (ovf << 16) + cnt;
}

uint32_t delay_millis(void)
{
    return delay_micros() / 1000;
}