#ifndef PIN_MAP_H
#define PIN_MAP_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"


#if((MCU_DEVICE == STM32F103CB)||(MCU_DEVICE == STM32F103C8)&&(MODULE == AI_2))
    #include "pin_map_stm32f103_ai_2.h"
#elif()

#endif // MCU_DEVICE & MODULE

#endif // PIN_MAP_H
