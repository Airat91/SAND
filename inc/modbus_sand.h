/*
 * File:        modbus_sand.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS API functions for SAND integration
 * Revision history: 0.1
 */

#ifndef MODBUS_SAND_H
#define MODBUS_SAND_H
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
 * @defgroup modbus
 * @brief Functions for ModBUS
 */

//--------Defines--------

#define MODBUS_TASK_PERIOD  10

//--------Macro--------

//--------Typedefs-------

//-------External variables------

extern osThreadId modbus_task_handle;
extern u32 modbus_led_ok_on_time;
extern u32 modbus_led_err_on_time;

//-------Function prototypes----------

void modbus_task(void const * argument);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MODBUS_SAND_H
