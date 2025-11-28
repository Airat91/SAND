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

//-------Functions----------

void rtc_task(void const * argument){
    (void)argument;
    if(rtc_init() < 0){
        debug_msg(__func__, DBG_MSG_ERR, "Cannot init RTC");
    }else{
        service.vars.rtc_state |= SRV_ST_RUN;
        debug_msg(__func__, DBG_MSG_INFO, "RTC_task started");
    }
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        if(rtc_ctrl & RTC_CTRL_UPD_EN){
            rtc_update();
        }

        osDelayUntil(&last_wake_time, RTC_TASK_PERIOD);
    }
}

int rtc_init(void){
    int result = 0;
    rtc_osc_t rtc_osc;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    HAL_StatusTypeDef stat = HAL_OK;
    __HAL_RCC_BKP_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();

    // Initializes RCC Internal/External Oscillator (LSE, LSI) configuration structure definition
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RTC_OSC_TYPE;            // Use RTC Osc
#if(RTC_OSC_TYPE == RCC_OSCILLATORTYPE_LSE)
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;                    // Enable Ext 32.768kHz RTC Osc
    rtc_osc = RTC_OSC_LSE;
#elif(RTC_OSC_TYPE == RCC_OSCILLATORTYPE_LSI)
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;                    // Enable Int 40kHz RTC RC
    rtc_osc = RTC_OSC_LSI;
#endif // RTC_OSC_TYPE

    stat = HAL_RCC_OscConfig(&RCC_OscInitStruct);
#if(RTC_OSC_TYPE == RCC_OSCILLATORTYPE_LSE)
    if(stat == HAL_TIMEOUT){
        // Reinit RTC_OSC by LSI
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
        RCC_OscInitStruct.LSIState = RCC_LSI_ON;
        stat = HAL_RCC_OscConfig(&RCC_OscInitStruct);
        if(stat == HAL_OK){
            rtc_osc = RTC_OSC_LSI;
            debug_msg(__func__, DBG_MSG_WARN, "RTC Inited with LSI instead LSE");
        }else{
            result = -1;
            debug_msg(__func__, DBG_MSG_ERR, "HAL_RCC_OscConfig() %S", hal_status[stat]);
        }
    }
#elif(RTC_OSC_TYPE == RCC_OSCILLATORTYPE_LSI)
    if(stat != HAL_OK){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_RCC_OscConfig() %S", hal_status[stat]);
    }
#endif // RTC_OSC_TYPE

    // Initializes RCC extended clocks structure definition
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    switch(rtc_osc){
    case RTC_OSC_LSE:
        PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
        break;
    case RTC_OSC_LSI:
        PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
        break;
    }
    stat = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
    if (stat != HAL_OK){
        result = -2;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_RCC_OscConfig() %S", hal_status[stat]);
    }


    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    stat = HAL_RTC_Init(&hrtc);
    if (stat != HAL_OK){
        result = -3;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_RTC_Init() %S", hal_status[stat]);
    }

    // If RTC isn't inited before set default time
    if(result == 0){
        stat = HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
        if(stat != HAL_OK){
            result = -4;
            debug_msg(__func__, DBG_MSG_ERR, "HAL_RTC_SetTime() %S", hal_status[stat]);
        }
    }

    // If RTC isn't inited before set default date
    if(result == 0){
        stat = HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
        if(stat != HAL_OK){
            result = -5;
            debug_msg(__func__, DBG_MSG_ERR, "HAL_RTC_SetDate() %S", hal_status[stat]);
        }
    }

    return result;
}

int rtc_deinit(void){
    int result = 0;
    __HAL_RCC_RTC_DISABLE();
    __HAL_RCC_LSE_CONFIG(RCC_LSE_OFF);

    return result;
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

int rtc_update(void){
    int result = 0;

    return result;
}

//-------Static functions----------


static int rtc_datetime_to_unix(rtc_date_t* date, rtc_time_t* time, u8 utc, u32* unix){
    int result = 0;

    return result;
}

static int rtc_unix_to_datetime(rtc_date_t* date, rtc_time_t* time, u8 utc, u32* unix){
    int result = 0;

    return result;
}
