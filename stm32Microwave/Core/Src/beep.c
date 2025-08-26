/******************************************************************************
 * @file    beep.c
 * @author  Yiran Zhang
 * @github  https://github.com/yz1295
 * @brief   Simple active-high/low buzzer driver for STM32 (HAL).
 ******************************************************************************/
#include "beep.h"

#ifdef BEEP_USE_DELAY_MS
extern void delay_ms(uint32_t ms);
#define BEEP_DELAY(ms) delay_ms(ms)
#else
#define BEEP_DELAY(ms) HAL_Delay(ms)
#endif

static void Beep_EnableGPIOClock(GPIO_TypeDef *port)
{
    if (port == GPIOA) { __HAL_RCC_GPIOA_CLK_ENABLE(); }
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

void Beep_Init(Beep_HandleTypeDef *hb, GPIO_TypeDef *port, uint16_t pin, GPIO_PinState active_level)
{
    if (!hb) return;

    hb->Port = port;
    hb->Pin = pin;
    hb->active_level = active_level;

    Beep_EnableGPIOClock(port);

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = pin;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(port, &gpio);

    // Default: OFF
    Beep_Off(hb);
}

void Beep_On(Beep_HandleTypeDef *hb)
{
    if (!hb) return;
    HAL_GPIO_WritePin(hb->Port, hb->Pin, hb->active_level);
}

void Beep_Off(Beep_HandleTypeDef *hb)
{
    if (!hb) return;
    HAL_GPIO_WritePin(hb->Port, hb->Pin, (hb->active_level == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void Beep_Toggle(Beep_HandleTypeDef *hb)
{
    if (!hb) return;
    HAL_GPIO_TogglePin(hb->Port, hb->Pin);
}

void Beep_Beep(Beep_HandleTypeDef *hb, uint8_t times, uint32_t on_ms, uint32_t off_ms)
{
    if (!hb || times == 0u) return;
    for (uint8_t i = 0; i < times; ++i) {
        Beep_On(hb);
        BEEP_DELAY(on_ms);
        Beep_Off(hb);
        if (i + 1u < times) {
            BEEP_DELAY(off_ms);
        }
    }
}
