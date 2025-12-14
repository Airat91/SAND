/*
 * File:        cmd_sand.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: SAND commands execution
 * Revision history: 0.1
 */

#include "cmd_sand.h"

//-------Global variables------

//-------Static variables------

//-------Static functions declaration-----------

//-------Functions----------

int cmd_sand_execute(u16* command_reg){
    int result = 0;
    u32 temp_start = 0;
    u32 temp_time = 0;

    cmd_sand_command_t cmd = *command_reg;

    switch(cmd){

    case CMD_NONE:
        break;

    case CMD_SAND_RESET_DEVICE:
    case CMD_SOFI_SW_RESET:
        debug_msg(__func__, DBG_MSG_INFO, "Reset device");
        temp_start = us_tim_get_value();
        main_shutdown_system();
        temp_time = us_tim_get_value() - temp_start;
        reset_system();
        break;

    case CMD_SAND_SAVE_DATA:
    case CMD_SOFI_SAVE_ALL_RETAINS:
    case CMD_SOFI_SAVE_BKRAM_TO_FLASH:
        debug_msg(__func__, DBG_MSG_INFO, "Save data");
        storage_save_data(&storage_pcb);
        break;

    case CMD_SAND_SET_DEFAULT_VALUES:
    case CMD_SOFI_SET_DEFAULT_VALUES:
        debug_msg(__func__, DBG_MSG_INFO, "Set default values");
        storage_set_defaults(&storage_pcb, sand_prop_save_list, SAND_PROP_SAVE_REG_NUM);
        storage_save_data(&storage_pcb);
        break;

    default:
        debug_msg(__func__, DBG_MSG_WARN, "Command code %d don't support", cmd);
    }

    // Clear command register
    *command_reg = 0;

    return result;
}

void cmd_sand_callback(void){
    cmd_sand_execute(&os.vars.command);
}

//-------Static functions----------
