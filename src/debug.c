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

//-------Functions----------

int debug_init(void){
    int result = 0;

    return result;
}

int debug_msg(const char* func_name, debug_msg_t msg_type, const char * msg, ...){
    int result = 0;

    char message[DEBUG_MSG_LEN] = {0};      // Formatted message
    char header[DEBUG_HEADER_LEN] = {0};    // Header of message
    va_list args;
    va_start(args, msg);
    u16 len = (u16)vsnprintf((char*)message, DEBUG_MSG_LEN, msg, args);
    va_end(args);
    if(len < DEBUG_MSG_LEN-1){
        message[len] = '\n';
        len++;
        message[len] = 0;
    }
#if RTC_EN
    // Add datetime to header
    sprintf(header, "%02d.%02d.%04d/%02d:%02d:%02d\t%s(): ", time.vars.day, time.vars.month, time.vars.year,
           time.vars.hour, time.vars.minute, time.vars.sec, func_name);
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

    printf("%s%s", header, message);

    return result;
}

//-------Static functions----------
