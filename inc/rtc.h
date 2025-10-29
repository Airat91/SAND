/* 
 * File:        rtc.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Functions for work with RTC periph of STM
 * Revision history: 0.1
 */

#ifndef RTC_H
#define	RTC_H

/*add includes below */
#include "stdint.h"
#include "stdlib.h"
#include "cmsis_os.h"
#include "type_def.h"
#include "main_config.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rtc.h"
#include "debug.h"
#include "sofi_reg.h"
/*add includes before */

#ifdef	__cplusplus
extern "C" {
#endif
    
/**
 * @defgroup rtc
 * @brief Functions for work with RTC
 */
    
//--------Defines--------

#define RTC_TASK_PERIOD     1000
#define RTC_INIT_TIMEOUT_MS 6000
#define RTC_OSC_TYPE        RCC_OSCILLATORTYPE_LSI  // Use RCC_OSCILLATORTYPE_LSE for external 32.768 kHz quartz
                                                    // or RCC_OSCILLATORTYPE_LSI for internal 40 kHz RC
    
//--------Macro--------

//--------Typedefs-------

typedef struct{
    u16 year;       // Year value
    u8 month;       // Month [1..12]
    u8 day;         // Day [1..31]
}rtc_date_t;

typedef struct{
    u8 hour;        // Hour [0..23]
    u8 minute;      // Minute [0..59]
    u8 sec;         // Second [0..59]
    u16 msec;       // Millisecond [0..999]
}rtc_time_t;

typedef enum{
    RTC_CTRL_UPD_EN     = (1<<0),   // Enable RTC update every 1 second
}rtc_ctrl_t;

typedef enum{
    RTC_OSC_LSI,    // Internal RC 40 kHz
    RTC_OSC_LSE,    // External quartz 32.768 kHz
}rtc_osc_t;

//-------External variables------

extern RTC_HandleTypeDef hrtc;
extern osThreadId rtc_task_handle;
extern rtc_ctrl_t rtc_ctrl;

//-------Function prototypes----------

/**
 * @brief rtc_task
 * @param argument
 * @ingroup rtc
 */
void rtc_task(void const * argument);

/**
 * @brief rtc_set_date
 * @param date
 * @ingroup rtc
 * @return
 *
 * Call this function after regs write by ModBUS
 */
int rtc_set_date(rtc_date_t date);

/**
 * @brief rtc_set_time
 * @param time
 * @ingroup rtc
 * @return
 *
 * Call this function after regs write by ModBUS
 */
int rtc_set_time(rtc_time_t time);

/**
 * @brief rtc_set_unix
 * @param unix_timestamp
 * @ingroup rtc
 * @return
 *
 * Call this function after regs write by ModBUS
 */
int rtc_set_unix(u32 unix_timestamp);

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */
