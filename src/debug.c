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

static debug_dynamic_buff_t debug_buf = {0};
static const char debug_msg_list[DBG_MSG_LIST_LEN][DEBUG_MSG_TYPE_LEN] = {
    "NON",
    "INF",      // Information message
    "WAR",      // Warning message
    "ERR",      // Error message with error LED blink
    "LOG",      // Save message to log-file
};

//-------Static functions declaration-----------

static int debug_send(char* p_msg, u16 len);
static int debug_buf_write(char *string, u16 len, u32 us_timeout);

//-------Functions----------

int debug_init(void){
    int result = 0;

    // Clear debug_buf
    memset(debug_buf.buff, 0, DEBUG_MSG_BUF_LEN * sizeof(u8));
    debug_buf.ptr = 0;
    debug_buf.start_for_read = 0;
    debug_buf.end_for_read = 0;
    debug_buf.msg_cnt = 0;
    debug_buf.state = DBG_BUF_READY;

    // Init external interface

    // Set dbg_service flags
    service.vars.dbg_state |= SRV_ST_CREATED;
    service.vars.dbg_state |= SRV_ST_RUN;
    service.vars.dbg_state &= ~(u32)SRV_ST_ERROR;

    return result;
}

int debug_msg(const char* func_name, debug_msg_t msg_type, const char * msg, ...){
    int result = 0;
    char message[DEBUG_MSG_LEN];      // Formatted message
    char header[DEBUG_HEADER_LEN];    // Header of message
    if(service_is_ok(&service.vars.dbg_state) == 0){
        va_list args;
        va_start(args, msg);
        u16 len = (u16)vsprintf((char*)message, msg, args);
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
                sprintf(header, "%02d.%02d.%04d/%02d:%02d:%02d\t%s\t%s(): ", time.vars.day, time.vars.month, time.vars.year,
                   time.vars.hour, time.vars.minute, time.vars.sec, debug_msg_list[msg_type], func_name);
            }else{
                // Add msec value to header
                sprintf(header, "%dms\t%s\t%s(): ", HAL_GetTick(), debug_msg_list[msg_type], func_name);
            }
#else
            // Add runtime to header
            sprintf(header, "%ds\t%s\t%s(): ", (int)os.vars.runtime, debug_msg_list[msg_type], func_name);
#endif // RTC_EN
            switch(msg_type){
            case DBG_MSG_INFO:
                break;
            case DBG_MSG_LOG:
                // Save message to FLASH
                break;
            case DBG_MSG_WARN:
                break;
            case DBG_MSG_ERR:
                // Blink LED error
                debug_led_err_on_time = DEBUG_LED_ERR_ON_TIME;
                break;
            default:
                break;
            }

            // Write message to debug_buf
            debug_buf_write(header, strlen(header), DEBUG_BUF_WRITE_TIMEOUT_US);
            debug_buf_write(message, len, DEBUG_BUF_WRITE_TIMEOUT_US);
            debug_buf.msg_cnt++;

            // Send message to external interface
            debug_send(header, strlen(header));
            debug_send(message, strlen(header));
        }
    }else{
        // Debug service is not ok
        if(msg_type == DBG_MSG_ERR){
            // Blink LED error
            debug_led_err_on_time = DEBUG_LED_ERR_ON_TIME;
        }
        result = -1;
    }

    return result;
}

int debug_buf_read(char *string, u16 read_len, u32 ms_timeout){
    int result = 0;

    u16 end_for_read = 0;
    if(debug_buf.end_for_read == debug_buf.start_for_read){
        // Buffer is empty
        result = 0;
    }else{
        while((debug_buf.state & DBG_BUF_READING)&&(ms_timeout)){
            osDelay(1);
            ms_timeout--;
        }
        if(ms_timeout == 0){
            // Time is out
            result = -1;
        }else{
            // Set debug_buf reading flag
            debug_buf.state |= DBG_BUF_READING;
            if(read_len > debug_buf_get_len()){
                // If debug_buf consist data less than read_len
                end_for_read = debug_buf.end_for_read;
            }else{
                // If debug_buf consist data more than read_len
                end_for_read = debug_buf.start_for_read + read_len;
                // If read data consist of 2 peaces
                if(end_for_read > DEBUG_MSG_BUF_LEN){
                    end_for_read -= DEBUG_MSG_BUF_LEN;
                }
            }
            if(end_for_read > debug_buf.start_for_read){
                // [######## Read debug message ######]
                //           ^start            ^end
                result = end_for_read - debug_buf.start_for_read;
                memcpy(string, &debug_buf.buff[debug_buf.start_for_read],(u16)result);
            }else{
                // [message ############## Read debug ]
                //         ^end            ^start
                u16 piece_1 = DEBUG_MSG_BUF_LEN - debug_buf.start_for_read;
                u16 piece_2 = end_for_read;
                result = piece_1 + piece_2;
                memcpy(string, &debug_buf.buff[debug_buf.start_for_read],(u16)piece_1);
                memcpy((string + piece_1), &debug_buf.buff[0],(u16)piece_2);
            }
            // move start_for_read pointer
            debug_buf.start_for_read = end_for_read;
            if(debug_buf.start_for_read == DEBUG_MSG_BUF_LEN){
                debug_buf.start_for_read = 0;
            }
            // Clear debug_buf reading flag
            debug_buf.state &= ~DBG_BUF_READING;
        }
    }
    return result;
}

void debug_err_led_on(void){
    debug_led_err_on_time = 1000;
}

int debug_buf_get_len(void){
    int result = 0;
    if(debug_buf.end_for_read == debug_buf.start_for_read){
        //buffer is empty
        result = 0;
    }else if(debug_buf.end_for_read > debug_buf.start_for_read){
        result = debug_buf.end_for_read - debug_buf.start_for_read;
    }else{
        u16 piece_1 = DEBUG_MSG_BUF_LEN - debug_buf.start_for_read;
        u16 piece_2 = debug_buf.end_for_read;
        result = piece_1 + piece_2;
    }
    return result;
}

//-------Static functions----------

/**
 * @brief Send debug message to interface
 * @param p_msg - pointer of message string
 * @param len - message string length
 * @ingroup debug
 * @return  0 - ok,\n
 *          negative value if error
 */
static int debug_send(char* p_msg, u16 len){
    int result = 0;
    // Add sendig data to debug interface

    return result;
}

/**
 * @brief Write message in debug_buf (FIFO)
 * @param string - pointer to log_message string
 * @param len - lenght of log_message
 * @param ms_timeout - timeout for waiting if buffer busy
 * @return  0 - OK,\n
 *          -1 - timeout error,\n
 *          -2 - len > DEBUG_DYNAMIC_BUFF_SIZE
 * @ingroup debug
 * @todo add message_cnt and timestamp into begin
 *
 */
static int debug_buf_write(char *string, u16 len, u32 ms_timeout){
    int result = 0;

    u8 shift_start_for_read = 0;

    if(len > DEBUG_MSG_BUF_LEN - 1){
        result = -2;    // Message is too long
    }else{
        while((debug_buf.state & DBG_BUF_WRITING)&&(ms_timeout)){
            osDelay(1);
            ms_timeout--;
        }
        if(ms_timeout == 0){
            result = -1;    // Time is out
        }else{
            // Set debug_buf writing flag
            debug_buf.state |= DBG_BUF_WRITING;

            if(len <= DEBUG_MSG_BUF_LEN - debug_buf.ptr){
                // [######## Place debug message here ######]
                //           ^start                  ^end
                if((debug_buf.ptr < debug_buf.start_for_read)&&\
                        (debug_buf.start_for_read < debug_buf.ptr + len + 1)){
                    // If new message cover start_for_read position, shift start_for_read position
                    shift_start_for_read = 1;
                }
                memcpy(&debug_buf.buff[debug_buf.ptr], string, len);
                debug_buf.ptr += len;
                debug_buf.end_for_read = debug_buf.ptr;
                if(debug_buf.ptr == DEBUG_MSG_BUF_LEN){
                    debug_buf.ptr = 0;
                }
            }else{
                // [message here ############## Place debug ]
                //              ^end            ^start
                u16 piece_1 = DEBUG_MSG_BUF_LEN - debug_buf.ptr;
                u16 piece_2 = len - piece_1;
                if((debug_buf.ptr < debug_buf.start_for_read)||\
                       (debug_buf.start_for_read < piece_2 + 1)){
                    // If new message cover start_for_read position, shift start_for_read position
                    shift_start_for_read = 1;
                }
                memcpy(&debug_buf.buff[debug_buf.ptr], string, piece_1);
                memcpy(&debug_buf.buff[0], (string + piece_1), piece_2);
                debug_buf.ptr = piece_2;
                debug_buf.end_for_read = debug_buf.ptr;
            }
            if(shift_start_for_read){
                // If new message cover start_for_read position, shift start_for_read position
                debug_buf.start_for_read = debug_buf.ptr + 1;
                if(debug_buf.start_for_read == DEBUG_MSG_BUF_LEN){
                    debug_buf.start_for_read = 0;
                }
            }
            //debug_buf.msg_cnt++;                    // Increment message counter
            debug_buf.state &= ~DBG_BUF_WRITING;   // Clear debug_buf writing flag
        }
    }
    return result;
}
