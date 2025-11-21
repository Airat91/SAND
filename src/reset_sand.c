/*
 * File:        reset_sand.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Reset status reading
 * Revision history: 0.1
 */

#include "reset_sand.h"

//-------Global variables------

//-------Static variables------

//-------Static functions declaration-----------

//-------Functions----------


int reset_get_reason(reset_sand_reason_t* reason){
    int result = 0;

    u32 rst_csr = RCC->CSR;
    *reason = 0;

    if(rst_csr & RCC_CSR_PINRSTF){
        *reason |= RST_REASON_PIN;
    }
    if(rst_csr & RCC_CSR_PORRSTF){
        *reason |= RST_REASON_PWR_ON;
    }
    if(rst_csr & RCC_CSR_SFTRSTF){
        *reason |= RST_REASON_SOFTWARE;
    }
    if(rst_csr & RCC_CSR_IWDGRSTF){
        *reason |= RST_REASON_IWDG;
    }
    if(rst_csr & RCC_CSR_WWDGRSTF){
        *reason |= RST_REASON_WWDG;
    }
    if(rst_csr & RCC_CSR_LPWRRSTF){
        *reason |= RST_REASON_PWR_OFF;
    }
    // Clear RCC_CSR register
    RCC->CSR |= RCC_CSR_RMVF;

    return result;
}

//-------Static functions----------
