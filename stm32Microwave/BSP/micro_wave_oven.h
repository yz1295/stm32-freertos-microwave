#ifndef MICRO_WAVE_OVEN_H_
#define MICRO_WAVE_OVEN_H_

/******************************************************
* Project : Microwave Oven Controller (STM32F407 + HAL)
* Author  : Yiran Zhang
* GitHub  : https://github.com/yz1295/stm32-freertos-microwave
* Date    : 2025/03/20
* Note    : HAL-based interface and data model
******************************************************/

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "led.h"

/* External LED descriptor (defined elsewhere) */
extern led_d led1;

/* ===== Timer handles provided by CubeMX =====
   - TIM2_CH2 (PA1) : door servo (50 Hz)
   - TIM3_CH3 (PB0) : heater PWM (1 kHz)
   - TIM3_CH4 (PC9) : turntable PWM (1 kHz)
   - TIM4          : 1 Hz base timer for countdown (Update IT)
*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

/* Handy aliases so code is self-documenting */
#define MW_DOOR_TIM         (&htim2)
#define MW_DOOR_CH          TIM_CHANNEL_2      /* PA1 */
#define MW_HEATER_TIM       (&htim3)
#define MW_HEATER_CH        TIM_CHANNEL_3      /* PB0 */
#define MW_TURNTABLE_TIM    (&htim3)
#define MW_TURNTABLE_CH     TIM_CHANNEL_4      /* PC9 */

/* Servo pulse (µs) for your SG90 — tune to your mechanics if needed */
#define DOOR_OPEN_US        (1000u)
#define DOOR_CLOSE_US       (2000u)
#define DOOR_NEUTRAL_US     (1500u)

/* Microwave states */
typedef enum {
    STATE_STANDBY = 0,    /* Idle */
    STATE_TIME_SETTING,   /* Set time */
    STATE_POWER_SETTING,  /* Set power */
    STATE_COMPLETED       /* Done */
} MicrowaveState;

/* Power levels */
typedef enum {
    POWER_LOW = 0,
    POWER_MEDIUM,
    POWER_HIGH
} PowerLevel;

/* Door state (logical) */
typedef enum {
    DOOR_OPEN = 0,
    DOOR_CLOSED = 1
} DoorState;

/* Heating flag */
typedef enum {
    HEATING_OFF = 0,
    HEATING_ON  = 1
} HeatingState;

/* Main control structure */
typedef struct {
    MicrowaveState state;     /* current state */
    uint16_t       cooking_time; /* seconds remaining */
    PowerLevel     power;     /* selected power */
    DoorState      door;      /* 0 = open, 1 = closed */
    HeatingState   heating;   /* 0/1 */
} MicrowaveCtrl;

/* API */
void micro_wave_init(MicrowaveCtrl *mw);
void plan_cooking(void);                     /* close door + turn panel LED off */
void end_cooking(void);                      /* open door + turn panel LED on  */
void start_cooking(MicrowaveCtrl *mw);
void stop_cooking(MicrowaveCtrl *mw);
void power_display(MicrowaveCtrl *mw);

/* Optional tickless hooks (FreeRTOS) */
#if configUSE_TICKLESS_IDLE
void PRE_SLEEP_PROCESSING(uint32_t ulExpectedIdleTime);
void POST_SLEEP_PROCESSING(uint32_t ulExpectedIdleTime);
#endif

#endif /* MICRO_WAVE_OVEN_H_ */
