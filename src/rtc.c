/* 
 * File:        rtc.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Functions for work with RTC periph of STM
 * Revision history: 0.1
 */

#include "rtc.h"

//-------Global variables------

RTC_HandleTypeDef hrtc;
osThreadId rtc_task_handle = {0};
rtc_ctrl_t rtc_ctrl = 0;

//-------Static variables------

//-------Static functions declaration-----------

static int rtc_init(void);
static int rtc_deinit(void);
static int rtc_update(void);

//-------Functions----------

void rtc_task(void const * argument){
    (void)argument;
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};
    if(rtc_init() < 0){
        debug_msg(__func__, DBG_MSG_ERR, "Cannot init RTC");
    }
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        if(rtc_ctrl & RTC_CTRL_UPD_EN){
            rtc_update();
        }

        osDelayUntil(&last_wake_time, RTC_TASK_PERIOD);
    }
}

int rtc_set_date(rtc_date_t date){
    int result = 0;
    RTC_DateTypeDef sDate = {0};
    HAL_StatusTypeDef stat = HAL_OK;

    sDate.Date = date.day;
    sDate.Month = date.month;
    sDate.Year = (uint8_t)(date.year - 2000);

    stat = HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    if(stat != HAL_OK){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_RTC_SetDate() %S", hal_status[stat]);
    }

    return result;
}

int rtc_set_time(rtc_time_t time){
    int result = 0;
    RTC_TimeTypeDef sTime = {0};
    HAL_StatusTypeDef stat = HAL_OK;

    sTime.Hours = time.hour;
    sTime.Minutes = time.minute;
    sTime.Seconds = time.sec;

    stat = HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    if(stat != HAL_OK){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_RTC_SetTime() %S", hal_status[stat]);
    }

    return result;
}

int rtc_set_unix(u32 unix_timestamp){
    int result = 0;

    return result;
}

int rtc_irq_handler(HAL_RTCStateTypeDef* hrtc){
    int result = 0;

    return result;
}


/**
 * @brief default_task
 * @param argument - None
 * @todo add group
 */


//-------Static functions----------

static int rtc_init(void){
    int result = 0;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    HAL_StatusTypeDef stat = HAL_OK;
    __HAL_RCC_BKP_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    stat = HAL_RTC_Init(&hrtc);
    if (stat != HAL_OK){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_RTC_Init() %S", hal_status[stat]);
    }
    if(result == 0){
        stat = HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
        if(stat != HAL_OK){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "HAL_RTC_SetTime() %S", hal_status[stat]);
        }
    }
    if(result == 0){
        stat = HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
        if(stat != HAL_OK){
            result = -3;
            debug_msg(__func__, DBG_MSG_ERR, "HAL_RTC_SetDate() %S", hal_status[stat]);
        }
    }

    return result;
}

static int rtc_deinit(void){
    int result = 0;

    return result;
}

static int rtc_update(void){
    int result = 0;

    return result;
}

static int rtc_datetime_to_unix(rtc_date_t* date, rtc_time_t* time, u8 utc, u32* unix){
    int result = 0;

    return result;
}

static int rtc_unix_to_datetime(rtc_date_t* date, rtc_time_t* time, u8 utc, u32* unix){
    int result = 0;

    return result;
}
