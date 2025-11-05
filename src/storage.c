/*
 * File:        storage.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Work with sand storage
 * Revision history: 0.1
 */

#include "storage.h"

//-------Global variables------

storage_pcb_t storage_pcb = {0};
osMutexId regs_storage_mutex = {0};

//-------Static variables------

static CRC_HandleTypeDef hcrc = {0};

//-------Static functions declaration-----------

//-------Functions----------

int storage_init(storage_pcb_t* storage_pcb, const sand_prop_save_t* reg_list, u16 reg_list_len){
    int result = 0;

    return result;
}

int storage_restore_data(storage_pcb_t* storage_pcb, const sand_prop_save_t* reg_list, u16 reg_list_len){
    int result = 0;

    return result;
}

int storage_save_data(storage_pcb_t* storage_pcb, const sand_prop_save_t* reg_list, u16 reg_list_len){
    int result = 0;

    return result;
}

int storage_data_changed_check(storage_pcb_t* storage_pcb, const sand_prop_save_t* reg_list, u16 reg_list_len){
    int result = 0;

    return result;
}

//-------Static functions----------

/**
 * @brief Init HardWare CRC calculation unit
 * @param hcrc - pointer to CRC_HandleTypeDef of CRC
 * @ingroup storage
 * @return 0
 */
static int storage_crc_init(CRC_HandleTypeDef* hcrc){
    int result = 0;

    __HAL_RCC_CRC_CLK_ENABLE();

    hcrc->Instance = CRC;
    HAL_CRC_Init(hcrc);
    __HAL_CRC_DR_RESET(hcrc);

    return result;
}
