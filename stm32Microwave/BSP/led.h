#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"

/******************************************************
* LED Driver (Header)
* Author  : Yiran Zhang
* GitHub  : https://github.com/yz1295/stm32-freertos-microwave
* Date    : 2025/03/20
* Note    : Simple HAL-based GPIO LED driver for STM32F4
******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* LED descriptor */
typedef struct {
    GPIO_TypeDef *port;   /* GPIO port, e.g., GPIOD */
    uint16_t      pin;    /* GPIO pin mask, e.g., GPIO_PIN_12 */
} led_d;

/**
 * @brief Initialize an LED descriptor and configure the pin as push-pull output.
 *        The function enables the GPIO clock automatically.
 * @param led  Pointer to LED descriptor
 * @param port GPIO port (GPIOA..GPIOI depending on MCU)
 * @param pin  GPIO pin mask (GPIO_PIN_x)
 */
void LED_Init(led_d *led, GPIO_TypeDef *port, uint16_t pin);

/**
 * @brief (Re)configure the LED pin as push-pull output (NOPULL, LOW speed).
 *        Safe to call multiple times.
 * @param led Pointer to LED descriptor
 */
void led_config(led_d *led);

/**
 * @brief Turn LED on (logic-high).
 * @param led Pointer to LED descriptor
 */
void led_on(led_d *led);

/**
 * @brief Turn LED off (logic-low).
 * @param led Pointer to LED descriptor
 */
void led_off(led_d *led);

/**
 * @brief Toggle LED state.
 * @param led Pointer to LED descriptor
 */
void led_toggle(led_d *led);

/* -------- Optional convenience helpers -------- */

/**
 * @brief Blink LED once using HAL_Delay (blocking).
 * @param led      Pointer to LED descriptor
 * @param on_ms    Time in milliseconds to keep LED on
 * @param off_ms   Time in milliseconds to keep LED off
 */
void led_blink_blocking(led_d *led, uint32_t on_ms, uint32_t off_ms);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */
