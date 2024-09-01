#include "sand_config.h"
#if ((FLASH_INT_EN == 1)||(FLASH_EXT_EN == 1))
#include "flash.h"

/*

static void save_params(void){
    int area_cnt = find_free_area();
    if(area_cnt < 0){
        uint32_t erase_error = 0;
        FLASH_EraseInitTypeDef flash_erase = {0};
        flash_erase.TypeErase = FLASH_TYPEERASE_PAGES;
        flash_erase.NbPages = 1;
        flash_erase.PageAddress = FLASH_SAVE_PAGE_ADDRESS;
        HAL_FLASH_Unlock();
        HAL_FLASHEx_Erase(&flash_erase, &erase_error);
        HAL_FLASH_Lock();
        area_cnt = 0;
    }
    for(uint8_t i = 0; i < SAVED_PARAMS_SIZE; i ++){
        save_to_flash(area_cnt, i, &config.word[i]);
    }
    // rewrite new params
    dcts.dcts_address = (uint8_t)config.params.mdb_address;
    uart_deinit();
    uart_init(config.params.mdb_bitrate, 8, 1, PARITY_NONE, 10000, UART_CONN_LOST_TIMEOUT);
    //delay for show message
    osDelay(2000);
}

static void restore_params(void){
    int area_cnt = find_free_area();
    if(area_cnt != 0){
        if(area_cnt == -1){
            // page is fill, actual values in last area
            area_cnt = SAVE_AREA_NMB - 1;
        }else{
            // set last filled area number
            area_cnt--;
        }
        uint16_t *addr;
        addr = (uint32_t)(FLASH_SAVE_PAGE_ADDRESS + area_cnt*SAVE_AREA_SIZE);
        for(uint8_t i = 0; i < SAVED_PARAMS_SIZE; i++){
            config.word[i] = *addr;
            addr++;
        }
    }else{
        //init default values if saved params not found
        config.params.mdb_address = dcts.dcts_address;
        config.params.mdb_bitrate = BITRATE_115200;
        config.params.light_lvl = 20;
        config.params.skin = HIGH_T_AND_TIME;
        config.params.data_pin_config = DATA_PIN_DISABLE;
        config.params.tmpr_coef_a = 100;
        config.params.tmpr_coef_b = 0;
    }
    for(bitrate_array_pointer = 0; bitrate_array_pointer < 14; bitrate_array_pointer++){
        if(bitrate_array[bitrate_array_pointer] == config.params.mdb_bitrate){
            break;
        }
    }
}
*/

#endif // ((FLASH_INT_EN == 1)||(FLASH_EXT_EN == 1))
