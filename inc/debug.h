/*
 * File:        debug.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: System debug functions and control
 * Revision history: 0.1
 */

#ifndef DEBUG_H
#define DEBUG_H
/*add includes below */
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "type_def.h"
#include "pin_map.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup debug
 * @brief System debug
 */

//--------Defines--------

//--------Macro--------

//--------Typedefs-------

//-------External variables------

extern u32 debug_led_err_on_time;

//-------Function prototypes----------

void modbus_task(void const * argument);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DEBUG_H
