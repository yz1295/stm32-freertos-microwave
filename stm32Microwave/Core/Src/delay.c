/**
  ******************************************************************************
  * @file    delay.c
  * @author  Yiran Zhang
  * @github  https://github.com/yz1295
  * @brief   Portable micro/millisecond delay utilities for STM32 (HAL).
  ******************************************************************************
  */

#include "delay.h"

/* Internal state */
static uint8_t  s_dwt_ok = 0;
static uint32_t s_cycles_per_us = 0;

/* ---------- DWT backend ---------- */

static uint8_t dwt_try_init(void)
{
    /* Enable trace subsystem so DWT runs */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Reset and enable cycle counter */
    DWT->CYCCNT = 0;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;

    /* Simple runtime test: does CYCCNT increment? */
    uint32_t before = DWT->CYCCNT;
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    uint32_t after  = DWT->CYCCNT;

    if (after != before)
    {
        s_cycles_per_us = SystemCoreClock / 1000000U;
        return 1;
    }
    return 0;
}

/* ---------- SysTick fallback (busy-wait in core cycles) ----------
   Assumes SysTick clock source is HCLK (Cube/HAL default).          */

static void systick_delay_us(uint32_t us)
{
    const uint32_t need_cycles = (SystemCoreClock / 1000000U) * us;
    const uint32_t load        = SysTick->LOAD + 1U;   /* counts per wrap */
    uint32_t acc   = 0;
    uint32_t prev  = SysTick->VAL;                     /* down-counting */

    while (acc < need_cycles)
    {
        uint32_t now = SysTick->VAL;
        uint32_t delta = (prev >= now) ? (prev - now) : (prev + load - now);
        acc  += delta;
        prev  = now;
    }
}

/* ---------- Public API ---------- */

uint8_t delay_init(void)
{
    s_dwt_ok = dwt_try_init();
    return s_dwt_ok;
}

void delay_us(uint32_t us)
{
    if (us == 0U) return;

    if (s_dwt_ok)
    {
        const uint32_t start  = DWT->CYCCNT;
        const uint32_t target = s_cycles_per_us * us;
        while ((DWT->CYCCNT - start) < target) { __NOP(); }
    }
    else
    {
        /* SysTick-based busy wait */
        systick_delay_us(us);
    }
}

void delay_ms(uint32_t ms)
{
    if (ms == 0U) return;

    if (s_dwt_ok)
    {
        /* Loop in 1 ms chunks to avoid overflow */
        while (ms--) delay_us(1000U);
    }
    else
    {
        /* Use HAL’s 1 kHz tick for accuracy & power friendliness */
        HAL_Delay(ms);
    }
}
