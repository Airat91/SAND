#ifndef RTC_H
#define RTC_H

#include "stm32f1xx_hal_rtc.h"


#define RTC_TASK_PERIOD 500

extern RTC_HandleTypeDef hrtc;


void rtc_task(void const * argument);

#endif // RTC_H
