#ifndef PIN_MAP_H
#define PIN_MAP_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"


#if(MODULE == AI_2)
    #include "pin_map_ai_2.h"
#elif()

#endif // MCU_DEVICE & MODULE

#endif // PIN_MAP_H
