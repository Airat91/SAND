/*
 * File:        mdb_sand.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS API functions for SAND integration
 * Revision history: 0.1
 */

#include "mdb_sand.h"

//-------Global variables------

osThreadId mdb_sand_task_handle = {0};
mdb_sand_pcb_t mdb_sand_pcb = {0};

//-------Static variables------

static const mdb_function_t mdb_sand_suprt_fnct_list[MDB_SUPRT_FNCT_NMB] = {
    MDB_FNCT_RD_MUL_HOLD,
    MDB_FNCT_RD_MUL_INPUT,
    MDB_FNCT_WR_MUL_COIL,
    MDB_FNCT_WR_MUL_HOLD,
};

//-------Static functions declaration-----------

static void mdb_sand_gpio_init(void);
static void mdb_sand_gpio_deinit(void);

//-------Functions----------

void mdb_sand_task(void const * argument){
    (void)argument;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){

        osDelayUntil(&last_wake_time, MDB_TASK_PERIOD);
    }
}

int mdb_sand_init(mdb_sand_pcb_t* mdb_sand_pcb){
    int result = 0;

    mdb_sand_gpio_init();
    mdb_sand_pcb->state |= MDB_ST_READY;

    return result;
}

int mdb_deinit(mdb_sand_pcb_t* mdb_sand_pcb){
    int result = 0;

    mdb_sand_gpio_deinit();
    mdb_sand_pcb->state = MDB_ST_DISABLE;

    return result;
}

int mdb_sand_read_addr(void){
    int result = 0;

    if(HAL_GPIO_ReadPin(MDB_ADDR_7_PORT, MDB_ADDR_7_PIN)){
        result |= 0x01;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_6_PORT, MDB_ADDR_6_PIN)){
        result |= 0x02;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_5_PORT, MDB_ADDR_5_PIN)){
        result |= 0x04;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_4_PORT, MDB_ADDR_4_PIN)){
        result |= 0x08;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_3_PORT, MDB_ADDR_3_PIN)){
        result |= 0x10;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_2_PORT, MDB_ADDR_2_PIN)){
        result |= 0x20;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_1_PORT, MDB_ADDR_1_PIN)){
        result |= 0x40;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_0_PORT, MDB_ADDR_0_PIN)){
        result |= 0x80;
    }

    return result;
}

//-------Static functions----------

/**
 * @brief Init switches GPIO of ModBUS
 * @ingroup modbus
 *
 * Init MDB_ADDR switch pins
 */
static void mdb_sand_gpio_init(void){
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
}

/**
 * @brief Deinit switches GPIO of ModBUS
 * @ingroup modbus
 *
 * Deinit MDB_ADDR switch pins
 */
static void mdb_sand_gpio_deinit(void){
    HAL_GPIO_DeInit(MDB_ADDR_0_PORT, MDB_ADDR_0_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_1_PORT, MDB_ADDR_1_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_2_PORT, MDB_ADDR_2_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_3_PORT, MDB_ADDR_3_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_4_PORT, MDB_ADDR_4_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_5_PORT, MDB_ADDR_5_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_6_PORT, MDB_ADDR_6_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_7_PORT, MDB_ADDR_7_PIN);
}

