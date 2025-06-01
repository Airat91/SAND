/*
 * File:        debug.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: System debug functions and control
 * Revision history: 0.1
 */

#include "debug.h"

//-------Global variables------

u32 debug_led_err_on_time = 0;

//-------Static variables------

//-------Static functions declaration-----------

static int debug_send(char* p_msg, u16 len);

//-------Functions----------

int debug_init(void){
    int result = 0;

    return result;
}

int debug_msg(const char* func_name, debug_msg_t msg_type, const char * msg, ...){
    int result = 0;
    char message[DEBUG_MSG_LEN] = {0};      // Formatted message
    char header[DEBUG_HEADER_LEN] = {0};    // Header of message
    if(service_is_ok(&service.vars.dbg_state) == 0){
        va_list args;
        va_start(args, msg);
        u16 len = (u16)vsnprintf((char*)message, DEBUG_MSG_LEN, msg, args);
        va_end(args);
        if(len >= DEBUG_MSG_LEN - 1){
            // Message is too long
            result = -2;
        }else{
            message[len] = '\n';
            len++;
            message[len] = 0;
#if RTC_EN
            if(service_is_ok(&service.vars.rtc_state) == 0){
                // Add datetime to header
                sprintf(header, "%02d.%02d.%04d/%02d:%02d:%02d\t%s(): ", time.vars.day, time.vars.month, time.vars.year,
                   time.vars.hour, time.vars.minute, time.vars.sec, func_name);
            }else{
                // Add msec value to header
                sprintf(header, "%dms\t%s(): ", HAL_GetTick(), func_name);
            }
#else
            // Add runtime to header
            sprintf(header, "%ds\t%s(): ", (int)os.vars.runtime, func_name);
#endif // RTC_EN
            switch(msg_type){
            case DBG_MSG_INFO:
                break;
            case DBG_MSG_LOG:
                break;
            case DBG_MSG_WARN:
                break;
            case DBG_MSG_ERR:
                break;
            default:
                break;
            }

            debug_send(header, strlen(header));
            debug_send(message, strlen(header));
        }
    }else{
        // Debug service is not ok
        result = -1;
    }

    return result;
}

//-------Static functions----------

/**
 * @brief Send debug message to interface
 * @param p_msg - pointer of message string
 * @param len - message string length
 * @return  0 - ok,\n
 *          negative value if error
 */
static int debug_send(char* p_msg, u16 len){
    int result = 0;

    return result;
}
