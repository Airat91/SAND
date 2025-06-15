/* 
 * File:        us_tim.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: 32-bit us timer api function
 * Revision history: 0.1
 */

#include "us_tim.h"

//-------Global variables------


//-------Static variables------

static uint32_t us_cnt_H = 0;
static TIM_HandleTypeDef htim_us;

//-------Static functions declaration-----------

//-------Functions----------

int us_tim_init(void){
    int result = 0;
    u32 stat = 0;

    __TM_US_CLK_ENABLE();                                               // Enable TIM CLK
    htim_us.Instance = TIM_US;                                          // Select TIM
    stat = HAL_RCC_GetPCLK1Freq()/US_TIM_FREQ - 1;
    htim_us.Init.Prescaler = stat;                                      // Set TI frequency = 1 MHz
    htim_us.Init.CounterMode = TIM_COUNTERMODE_UP;                      // Select up-ounter mode
    htim_us.Init.Period = 0xFFFF;                                       // Set max period
    htim_us.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                // Set TI frequency = 1 MHz
    htim_us.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;    // Disable autoreload mode
    stat = HAL_TIM_Base_Init(&htim_us);
    if (stat != HAL_OK){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_TIM_Base_Init() %S", hal_status[stat]);
    }

    TIM_ClockConfigTypeDef sClockSourceConfig;
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;          // Select CLK source
    sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;   // Select CLK polarity
    sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;        // Select CLK divider = 1
    sClockSourceConfig.ClockFilter = 0;                                 // Set CLK filter = 0

    stat = HAL_TIM_ConfigClockSource(&htim_us, &sClockSourceConfig);
    if (stat != HAL_OK){
        result = -2;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_TIM_ConfigClockSource() %S", hal_status[stat]);
    }

    TIM_MasterConfigTypeDef sMasterConfig;
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;                 // Reset trigger ouput mode
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;        // Disable master/slave mode
    stat = HAL_TIMEx_MasterConfigSynchronization(&htim_us, &sMasterConfig);
    if (stat != HAL_OK){
        result = -3;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_TIMEx_MasterConfigSynchronization() %S", hal_status[stat]);
    }

    // Enable IRQ for US_TIM
    HAL_NVIC_EnableIRQ(TIM_US_IRQn);

    // Start US_TIM
    stat = HAL_TIM_Base_Start_IT(&htim_us);
    if (stat != HAL_OK){
        result = -4;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_TIM_Base_Start_IT() %S", hal_status[stat]);
    }

    return result;
}

void us_tim_deinit(void){
    HAL_TIM_Base_Stop_IT(&htim_us);
    HAL_NVIC_DisableIRQ(TIM_US_IRQn);
    HAL_TIM_Base_DeInit(&htim_us);
}

u32 us_tim_get_value(void){
    u32 value = us_cnt_H + htim_us.Instance->CNT;
    return value;
}

void us_tim_delay(u32 us){
    u32 current;
    u8 with_yield;
    current = htim_us.Instance->CNT;
    with_yield = 0;
    if(us > US_TIM_YIELD_THRESHOLD){
        with_yield = 1;
    }
    while((htim_us.Instance->CNT - current) < us){
        if(with_yield){
            osThreadYield();
        }
    }
}

void us_tim_irq_handler(void){
    HAL_TIM_IRQHandler(&htim_us);
    us_cnt_H += 0x10000;
}

//-------Static functions----------
