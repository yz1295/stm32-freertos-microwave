/******************************************************
* LED Driver (Source)
* Author  : Yiran Zhang
* GitHub  : https://github.com/yz1295/stm32-freertos-microwave
* Date    : 2025/03/20
* Note    : Simple HAL-based GPIO LED driver for STM32F4
******************************************************/

#include "led.h"

/* Enable the GPIO clock for the given port pointer */
static void LED_EnableGPIOClock(GPIO_TypeDef *port)
{
    if (port == GPIOA)      { __HAL_RCC_GPIOA_CLK_ENABLE(); }
    else if (port == GPIOB) { __HAL_RCC_GPIOB_CLK_ENABLE(); }
    else if (port == GPIOC) { __HAL_RCC_GPIOC_CLK_ENABLE(); }
    else if (port == GPIOD) { __HAL_RCC_GPIOD_CLK_ENABLE(); }
    else if (port == GPIOE) { __HAL_RCC_GPIOE_CLK_ENABLE(); }
#ifdef GPIOF
    else if (port == GPIOF) { __HAL_RCC_GPIOF_CLK_ENABLE(); }
#endif
#ifdef GPIOG
    else if (port == GPIOG) { __HAL_RCC_GPIOG_CLK_ENABLE(); }
#endif
#ifdef GPIOH
    else if (port == GPIOH) { __HAL_RCC_GPIOH_CLK_ENABLE(); }
#endif
#ifdef GPIOI
    else if (port == GPIOI) { __HAL_RCC_GPIOI_CLK_ENABLE(); }
#endif
}

/* Public API implementations */

void LED_Init(led_d *led, GPIO_TypeDef *port, uint16_t pin)
{
    if (!led) return;

    led->port = port;
    led->pin  = pin;

    LED_EnableGPIOClock(port);
    led_config(led);
}

void led_config(led_d *led)
{
    if (!led) return;

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin   = led->pin;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;     /* push-pull output */
    gpio.Pull  = GPIO_NOPULL;             /* no pull-up/down */
    gpio.Speed = GPIO_SPEED_FREQ_LOW;     /* LED does not need high speed */

    HAL_GPIO_Init(led->port, &gpio);

    /* Default state: off */
    led_off(led);
}

void led_on(led_d *led)
{
    if (!led) return;
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}

void led_off(led_d *led)
{
    if (!led) return;
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

void led_toggle(led_d *led)
{
    if (!led) return;
    HAL_GPIO_TogglePin(led->port, led->pin);
}

void led_blink_blocking(led_d *led, uint32_t on_ms, uint32_t off_ms)
{
    if (!led) return;
    led_on(led);
    HAL_Delay(on_ms);
    led_off(led);
    HAL_Delay(off_ms);
}

