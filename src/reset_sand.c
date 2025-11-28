/*
 * File:        reset_sand.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Reset functions
 * Revision history: 0.1
 */

#include "reset_sand.h"

//-------Global variables------


//-------Static variables------

static u8 ready_for_shutdown = 0;

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

int reset_system(void){
    int result = 0;

    main_shutdown_system();
    NVIC_SystemReset();

    return result;
}

int reset_lowpower_init(void){
    int result = 0;

    __HAL_RCC_PWR_CLK_ENABLE();
    PWR_PVDTypeDef sConfigPVD   = {0};
    sConfigPVD.PVDLevel         = PWR_PVDLEVEL_7;           // 2.9V
    sConfigPVD.Mode             = PWR_PVD_MODE_IT_RISING;   // Interrupt on falling edge

    HAL_PWR_ConfigPVD(&sConfigPVD);

    // Clear interrupt flag
    __HAL_PWR_PVD_EXTI_CLEAR_FLAG();

    // Enable interrupt
    HAL_NVIC_SetPriority(PVD_IRQn, PVD_PRIO, PVD_SUBPRIO);
    HAL_NVIC_EnableIRQ(PVD_IRQn);

    HAL_PWR_EnablePVD();

    return result;
}

int reset_lowpower_irq_callback(void){
    int result = 0;

    if(__HAL_PWR_PVD_EXTI_GET_FLAG() != RESET){
        if(ready_for_shutdown == 1){
            main_shutdown_system();
            main_discharge_pwr();
            osDelay(RESET_SHUTDOWN_DELAY_MS);
            NVIC_SystemReset();
        }
    }
    __HAL_PWR_PVD_EXTI_CLEAR_FLAG();

    return result;
}

int reset_lowpower_save_enable(void){
    int result = 0;

    ready_for_shutdown = 1;

    return result;
}

//-------Static functions----------
