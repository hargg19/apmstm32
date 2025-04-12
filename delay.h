#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>
#include "stm32f10x.h"

/**
  * @brief Inisialisasi Timer 5 untuk delay presisi
  */
void delay_init(void);

/**
  * @brief Delay blocking dengan presisi milidetik
  * @param ms: waktu delay dalam milidetik (0-65535)
  */
void delay_ms(uint32_t ms);

/**
  * @brief Delay blocking dengan presisi mikrodetik
  * @param us: waktu delay dalam mikrodetik (0-65535)
  */
void delay_us(uint32_t us);

/**
  * @brief Get current tick in microseconds
  * @retval Nilai tick saat ini dalam mikrodetik
  */
uint32_t delay_micros(void);

/**
  * @brief Get current tick in milliseconds
  * @retval Nilai tick saat ini dalam milidetik
  */
uint32_t delay_millis(void);

#endif // DELAY_H