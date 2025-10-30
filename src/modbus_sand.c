/*
 * File:        modbus_sand.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS API functions for SAND integration
 * Revision history: 0.1
 */

#include "modbus_sand.h"

//-------Global variables------

osThreadId modbus_task_handle = {0};
modbus_pcb_t modbus_pcb = {0};

//-------Static variables------

static const modbus_function_t modbus_suprt_fnct_list[MODBUS_SUPRT_FNCT_NMB] = {
    MODBUS_FNCT_RD_MUL_HOLD,
    MODBUS_FNCT_RD_MUL_INPUT,
    MODBUS_FNCT_WR_MUL_COIL,
    MODBUS_FNCT_WR_MUL_HOLD,
};

//-------Static functions declaration-----------

static void modbus_gpio_init(void);
static void modbus_gpio_deinit(void);

//-------Functions----------

void modbus_task(void const * argument){
    (void)argument;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){

        osDelayUntil(&last_wake_time, MODBUS_TASK_PERIOD);
    }
}

int modbus_init(modbus_pcb_t* modbus_pcb){
    int result = 0;

    modbus_gpio_init();
    modbus_pcb->state |= MODBUS_ST_READY;

    return result;
}

int modbus_deinit(modbus_pcb_t* modbus_pcb){
    int result = 0;

    modbus_gpio_deinit();
    modbus_pcb->state = MODBUS_ST_DISABLE;

    return result;
}

int modbus_read_addr(void){
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

int modbus_packet_recognise(u8* buf, u16 len){
    int result = 0;

    return result;
}
//-------Static functions----------

/**
 * @brief Init switches GPIO of ModBUS
 * @ingroup modbus
 *
 * Init MDB_ADDR switch pins
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
}

/**
 * @brief Deinit switches GPIO of ModBUS
 * @ingroup modbus
 *
 * Deinit MDB_ADDR switch pins
 */
static void modbus_gpio_deinit(void){
    HAL_GPIO_DeInit(MDB_ADDR_0_PORT, MDB_ADDR_0_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_1_PORT, MDB_ADDR_1_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_2_PORT, MDB_ADDR_2_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_3_PORT, MDB_ADDR_3_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_4_PORT, MDB_ADDR_4_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_5_PORT, MDB_ADDR_5_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_6_PORT, MDB_ADDR_6_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_7_PORT, MDB_ADDR_7_PIN);
}

