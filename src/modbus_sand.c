/*
 * File:        modbus_sand.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS API functions for SAND integration
 * Revision history: 0.1
 */

#include "modbus_sand.h"

//-------Global variables------

osThreadId modbus_task_handle = {0};
u32 modbus_led_ok_on_time = 0;
u32 modbus_led_err_on_time = 0;

//-------Static variables------

//-------Static functions declaration-----------

static void modbus_gpio_init(void);

//-------Functions----------

void modbus_task(void const * argument){
    (void)argument;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){

        osDelayUntil(&last_wake_time, MODBUS_TASK_PERIOD);
    }
}

//-------Static functions----------

/**
 * @brief Init switches GPIO of ModBUS address and rate
 * @ingroup modbus
 *
 * Init MDB_ADDR switch pins
 * Init MDB_RATE switch pins
 */
static void modbus_gpio_init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;

    // Init MDB_ADDR switch pins
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = MDB_ADDR_0_PIN;
    HAL_GPIO_Init (MDB_ADDR_0_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_1_PIN;
    HAL_GPIO_Init (MDB_ADDR_1_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_2_PIN;
    HAL_GPIO_Init (MDB_ADDR_2_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_3_PIN;
    HAL_GPIO_Init (MDB_ADDR_3_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_4_PIN;
    HAL_GPIO_Init (MDB_ADDR_4_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_5_PIN;
    HAL_GPIO_Init (MDB_ADDR_5_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_6_PIN;
    HAL_GPIO_Init (MDB_ADDR_6_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_7_PIN;
    HAL_GPIO_Init (MDB_ADDR_7_PORT, &GPIO_InitStruct);

    // Init MDB_RATE switch pins
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = MDB_RATE_0_PIN;
    HAL_GPIO_Init (MDB_RATE_0_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_RATE_1_PIN;
    HAL_GPIO_Init (MDB_RATE_1_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_RATE_2_PIN;
    HAL_GPIO_Init (MDB_RATE_2_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_RATE_3_PIN;
    HAL_GPIO_Init (MDB_RATE_3_PORT, &GPIO_InitStruct);
}
