/**
  ******************************************************************************
  * @file    delay.h
  * @author  Yiran Zhang
  * @github  https://github.com/yz1295
  * @brief   Portable micro/millisecond delay utilities for STM32 (HAL).
  *          Uses DWT (cycle counter) when available; otherwise falls back to
  *          SysTick-based busy-wait and HAL_Delay for milliseconds.
  ******************************************************************************
  */

#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize delay backend.
 *         Call once after SystemClock_Config().
 *         Returns 1 if DWT is available/used, 0 if SysTick fallback is used.
 */
uint8_t delay_init(void);

/**
 * @brief  Busy-wait for the given number of microseconds.
 * @note   Uses DWT if available; otherwise uses a SysTick-based loop.
 */
void delay_us(uint32_t us);

/**
 * @brief  Delay for the given number of milliseconds.
 * @note   Uses HAL_Delay() for the fallback path; DWT path loops delay_us().
 */
void delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H */
