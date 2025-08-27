#ifndef MICRO_WAVE_OVEN_H_
#define MICRO_WAVE_OVEN_H_

/******************************************************
* Project : Microwave Oven Controller (FreeRTOS-based)
* Author  : Yiran Zhang
* GitHub  : https://github.com/yz1295/stm32-freertos-microwave
* Date    : 2025/03/20
* Note    : For educational use only
******************************************************/

#include "stm32f407xx.h"
#include "led.h"

/* External variables */
extern led_d led1;

/* Microwave operating states */
typedef enum {
    STATE_STANDBY,        // Standby mode (idle)
    STATE_TIME_SETTING,   // Time configuration mode
    STATE_POWER_SETTING,  // Power level configuration
    STATE_COMPLETED       // Heating completed
} MicrowaveState;

/* Power level options */
typedef enum {
    POWER_LOW,            // Low power
    POWER_MEDIUM,         // Medium power
    POWER_HIGH            // High power
} PowerLevel;

/* Door state */
typedef enum {
    DOOR_OPEN = 0,
    DOOR_CLOSED
} DoorState;

/* Heating status */
typedef enum {
    HEATING_OFF = 0,
    HEATING_ON
} HeatingState;

/* Microwave control structure */
typedef struct {
    MicrowaveState state;   // Current microwave state
    uint16_t cooking_time;  // Countdown timer (seconds)
    PowerLevel power;       // Selected power level
    DoorState door;         // Door state (open/closed)
    HeatingState heating;   // Heating status (on/off)
} MicrowaveCtrl;

/* Function prototypes */
void micro_wave_init(MicrowaveCtrl *microwave);
void plan_cooking(MicrowaveCtrl *microwave);
void start_cooking(MicrowaveCtrl *microwave);
void stop_cooking(MicrowaveCtrl *microwave);
void end_cooking(MicrowaveCtrl *microwave);
void power_display(MicrowaveCtrl *microwave);

/*------------------------------------------------*/
/*  FreeRTOS Tickless Idle Power-Save Hooks        */
/*------------------------------------------------*/
#if configUSE_TICKLESS_IDLE
    void PRE_SLEEP_PROCESSING(uint32_t ulExpectedIdleTime);
    void POST_SLEEP_PROCESSING(uint32_t ulExpectedIdleTime);
#endif
/*------------------------------------------------*/

#endif /* MICRO_WAVE_OVEN_H_ */
