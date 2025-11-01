/*
 * File:        debug.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: System debug functions and control
 * Revision history: 0.1
 */

#ifndef DEBUG_H
#define DEBUG_H
/*add includes below */
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "hal_status.h"
#include "type_def.h"
#include "pin_map.h"
#include "main_config.h"
#include "reg.h"
#include "service.h"
#include "us_tim.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup debug
 * @brief System debug
 */

//--------Defines--------

#define DEBUG_HEADER_LEN            100
#define DEBUG_MSG_LEN               200
#define DEBUG_MSG_BUF_LEN           1024
#define DEBUG_BUF_WRITE_TIMEOUT_US  1000
#define DEBUG_BUF_READ_TIMEOUT_US   1000
#define DEBUG_MSG_TYPE_LEN          5
#define DEBUG_LED_ERR_ON_TIME       100

//--------Macro--------

//--------Typedefs-------

typedef enum{
    DBG_MSG_NONE = 0,
    DBG_MSG_INFO,       // Information message
    DBG_MSG_WARN,       // Warning message
    DBG_MSG_ERR,        // Error message with error LED blink
    DBG_MSG_LOG,        // Save message to log-file
    DBG_MSG_LIST_LEN,   // Number of debug message types
}debug_msg_t;

typedef enum{
    DBG_BUF_NONE        = 0,
    DBG_BUF_READY       = 1<<0,
    DBG_BUF_WRITING     = 1<<1,
    DBG_BUF_READING     = 1<<2,
}debug_buff_state_t;

typedef struct{
    char buff[DEBUG_MSG_BUF_LEN];       // buffer for logs
    u16 ptr;                            // first empty symbol for new message
    u16 start_for_read;                 // first symbol for read
    u16 end_for_read;                   // end symbol for read including this
    u64 msg_cnt;                        // counter of log messages
    debug_buff_state_t state;           // state flags
}debug_dynamic_buff_t;

//-------External variables------

extern u32 debug_led_err_on_time;

//-------Function prototypes----------

/**
 * @brief Init debug service
 * @ingroup debug
 * @return  0 - ok,\n
 *          negative value - error
 */
int debug_init(void);

/**
 * @brief Write debug message
 * @param func_name - call function name
 * @param msg_type - see debug_msg_t
 * @param msg - message
 * @ingroup debug
 * @return  0 - ok,\n
 *          -1 - Debug service is not ok,\n
 *          -2 - Message is too long,\n
 *
 * @example debug_msg(__func__, DBG_MSG_INFO, "Hello world!")
 */
int debug_msg(const char* func_name, debug_msg_t msg_type, const char * msg, ...);

/**
 * @brief Read messages from debug_buf (FIFO)
 * @param string - pointer to copy log_message
 * @param read_len - len for reading
 * @param ms_timeout - timeout for waiting if buffer busy
 * @return  read_len,\n
 *          0 - buffer is epty,\n
 *          -1 - timeout error
 * @ingroup debug
 */
int debug_buf_read(char *string, u16 read_len, u32 ms_timeout);

/**
 * @brief Get len between start and end for read pointers
 * @return lenght of log_message
 * @ingroup debug
 */
int debug_buf_get_len(void);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DEBUG_H
