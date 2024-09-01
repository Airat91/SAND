#include "sand_config.h"
#if RTC_EN
#include "rtc.h"

/* RTC init function */
static void RTC_Init(void){
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    __HAL_RCC_BKP_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    if (HAL_RTC_Init(&hrtc) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }

    sTime.Hours = dcts.dcts_rtc.hour;
    sTime.Minutes = dcts.dcts_rtc.minute;
    sTime.Seconds = dcts.dcts_rtc.second;

    sDate.Date = dcts.dcts_rtc.day;
    sDate.Month = dcts.dcts_rtc.month;
    sDate.Year = (uint8_t)(dcts.dcts_rtc.year - 2000);

    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

/**
 * @brief RTC_set
 * @param dcts_rtc
 * @return
 */
int RTC_set(rtc_t dcts_rtc){
    int result = 0;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    sTime.Hours = dcts_rtc.hour;
    sTime.Minutes = dcts_rtc.minute;
    sTime.Seconds = dcts_rtc.second;

    sDate.Date = dcts_rtc.day;
    sDate.Month = dcts_rtc.month;
    sDate.Year = (uint8_t)(dcts_rtc.year - 2000);

    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    return result;
}


/**
 * @brief default_task
 * @param argument - None
 * @todo add group
 */
void rtc_task(void const * argument){

    (void)argument;
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};
    RTC_Init();
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        switch (dcts.dcts_rtc.state) {
        case RTC_STATE_READY:   //update dcts_rtc from rtc
            HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);
            HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);

            taskENTER_CRITICAL();
            dcts.dcts_rtc.hour = time.Hours;
            dcts.dcts_rtc.minute = time.Minutes;
            dcts.dcts_rtc.second = time.Seconds;

            dcts.dcts_rtc.day = date.Date;
            dcts.dcts_rtc.month = date.Month;
            dcts.dcts_rtc.year = date.Year + 2000;
            dcts.dcts_rtc.weekday = date.WeekDay;
            taskEXIT_CRITICAL();
            break;
        case RTC_STATE_SET:     //set new values from dcts_rtc
            time.Hours = dcts.dcts_rtc.hour;
            time.Minutes = dcts.dcts_rtc.minute;
            time.Seconds = dcts.dcts_rtc.second;

            date.Date = dcts.dcts_rtc.day;
            date.Month = dcts.dcts_rtc.month;
            date.Year = (uint8_t)(dcts.dcts_rtc.year - 2000);

            HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
            HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);

            dcts.dcts_rtc.state = RTC_STATE_READY;
            break;
        default:
            break;
        }
#if RELEASE
        HAL_IWDG_Refresh(&hiwdg);
#endif //RELEASE
        osDelayUntil(&last_wake_time, RTC_TASK_PERIOD);
    }
}


static void save_to_bkp(u8 bkp_num, u8 var){
    uint32_t data = var;
    if(bkp_num%2 == 1){
        data = data << 8;
    }
    HAL_PWR_EnableBkUpAccess();
    switch (bkp_num / 2){
    case 0:
        BKP->DR1 |= data;
        break;
    case 1:
        BKP->DR2 |= data;
        break;
    case 2:
        BKP->DR3 |= data;
        break;
    case 3:
        BKP->DR4 |= data;
        break;
    case 4:
        BKP->DR5 |= data;
        break;
    case 5:
        BKP->DR6 |= data;
        break;
    case 6:
        BKP->DR7 |= data;
        break;
    case 7:
        BKP->DR8 |= data;
        break;
    case 8:
        BKP->DR9 |= data;
        break;
    case 9:
        BKP->DR10 |= data;
        break;
    }
    HAL_PWR_DisableBkUpAccess();
}

static void save_float_to_bkp(u8 bkp_num, float var){
    char buf[5] = {0};
    sprintf(buf, "%4.0f", (double)var);
    u8 data = (u8)atoi(buf);
    save_to_bkp(bkp_num, data);
}
static u8 read_bkp(u8 bkp_num){
    uint32_t data = 0;
    switch (bkp_num/2){
    case 0:
        data = BKP->DR1;
        break;
    case 1:
        data = BKP->DR2;
        break;
    case 2:
        data = BKP->DR3;
        break;
    case 3:
        data = BKP->DR4;
        break;
    case 4:
        data = BKP->DR5;
        break;
    case 5:
        data = BKP->DR6;
        break;
    case 6:
        data = BKP->DR7;
        break;
    case 7:
        data = BKP->DR8;
        break;
    case 8:
        data = BKP->DR9;
        break;
    case 9:
        data = BKP->DR10;
        break;
    }
    if(bkp_num%2 == 1){
        data = data >> 8;
    }
    return (u8)(data & 0xFF);
}
static float read_float_bkp(u8 bkp_num, u8 sign){
    u8 data = read_bkp(bkp_num);
    char buf[5] = {0};
    if(sign == READ_FLOAT_SIGNED){
        sprintf(buf, "%d", (s8)data);
    }else{
        sprintf(buf, "%d", data);
    }
    return atoff(buf);
}

#endif // RTC_EN
