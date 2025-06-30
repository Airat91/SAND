/* 
 * File:        service.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Service tasks descriptions
 * Revision history: 0.1
 */

#include "service.h"

//-------Global variables------

//-------Static variables------

//-------Static functions declaration-----------

//-------Functions----------

int service_is_ok(u32* p_service_state){
    int result = 0;
    if((*p_service_state & SRV_ST_CREATED) == 0){
        // Service doesn't created
        result = -1;
    }else if(*p_service_state & SRV_ST_ERROR){
        // Service in ERROR state
        result = -2;
    }else if((*p_service_state & SRV_ST_RUN) == 0){
        // Service doesn't run, wait timeout
        u32 timeout = *p_service_state & SRV_ST_TIMEOUT_MASK;
        while(((*p_service_state & SRV_ST_RUN) == 0)&(timeout)){
            osDelay(1);
            timeout--;
        }
        if((*p_service_state & SRV_ST_RUN) == 0){
            // Service don't start, set ERROR state
            result = -3;
            *p_service_state |= SRV_ST_ERROR;
        }
    }

    return result;
}

//-------Static functions----------
