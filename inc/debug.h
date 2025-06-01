/*
 * File:        debug.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: System debug functions and control
 * Revision history: 0.1
 */

#ifndef DEBUG_H
#define DEBUG_H
/*add includes below */
#include <stdarg.h>
#include <string.h>
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "hal_status.h"
#include "type_def.h"
#include "pin_map.h"
#include "main_config.h"
#include "regs.h"
#include "service.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup debug
 * @brief System debug
 */

//--------Defines--------

#define DEBUG_HEADER_LEN    100
#define DEBUG_MSG_LEN       255

//--------Macro--------

//--------Typedefs-------

typedef enum{
    DBG_MSG_NONE = 0,
    DBG_MSG_INFO,       // Information message
    DBG_MSG_WARN,       // Warning message
    DBG_MSG_ERR,        // Error message with error LED blink
    DBG_MSG_LOG,        // Save message to log-file
}debug_msg_t;

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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DEBUG_H
