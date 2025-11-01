/*
 * File:        reg.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global registers access
 * Revision history: 0.1
 */

#include "reg.h"

//-------Global variables------

//-------Static variables------

//-------Static functions declaration-----------

//-------Functions----------

sofi_prop_base_t* reg_get_by_name(char* reg_name){
    sofi_prop_base_t* reg;
    int reg_found = 0;

    for(u16 i = 0; i < SOFI_PROP_BASE_REG_NUM; i++){
        reg = (sofi_prop_base_t*)&sofi_prop_base_list[i];
        if(strcmp(reg->name, reg_name) == 0){
            reg_found = 1;
            break;
        }
    }
    if(reg_found == 0){
        reg = NULL;
    }

    return reg;
}

sofi_prop_base_t* reg_get_by_ind(u16 ind){
    sofi_prop_base_t* reg;

    if(ind > SOFI_PROP_BASE_REG_NUM){
        reg = NULL;
    }else{
        reg = (sofi_prop_base_t*)&sofi_prop_base_list[ind];
    }

    return reg;
}

//-------Static functions----------
