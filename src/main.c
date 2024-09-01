
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/**
  * @defgroup MAIN
  */

#define RESET_HOLD 3000

typedef enum{
    READ_FLOAT_SIGNED = 0,
    READ_FLOAT_UNSIGNED,
}read_float_bkp_sign_t;


/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
IWDG_HandleTypeDef hiwdg;
osThreadId dutyTaskHandle;
osThreadId displayTaskHandle;
osThreadId adcTaskHandle;
osThreadId am2302TaskHandle;
osThreadId buttonsTaskHandle;
osThreadId navigationTaskHandle;
osThreadId uartTaskHandle;
uint8_t irq_state = IRQ_NONE;

u16 led_os_on_time = 0;
u16 led_os_error_on_time = 0;
u16 led_packet_send_time = 0;
u16 led_packet_recv_time = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_IWDG_Init(void);
//static void RTC_Init(void);
static void tim_us_init(void);
/*static void save_to_bkp(u8 bkp_num, u8 var);
static void save_float_to_bkp(u8 bkp_num, float var);
static u8 read_bkp(u8 bkp_num);
static float read_float_bkp(u8 bkp_num, u8 sign);*/
static void gpio_init(void);
/*static void data_pin_irq_init(void);
static void save_params(void);
static void restore_params(void);

static void print_main(void);
static void print_menu(void);
static void print_value(u8 tick);*/
static int leds_handle(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

uint32_t us_cnt_H = 0;
static edit_val_t edit_val = {0};
static navigation_t navigation_style = MENU_NAVIGATION;
saved_to_flash_t config;
/*static const uart_bitrate_t bitrate_array[14] = {
    BITRATE_600,
    BITRATE_1200,
    BITRATE_2400,
    BITRATE_4800,
    BITRATE_9600,
    BITRATE_14400,
    BITRATE_19200,
    BITRATE_28800,
    BITRATE_38400,
    BITRATE_56000,
    BITRATE_57600,
    BITRATE_115200,
    BITRATE_128000,
    BITRATE_256000,
};
static uint16_t bitrate_array_pointer = 0;
static const char skin_description[SKIN_NMB][20] = {
    "T TIME",
    "HIGH_T",
    "TIME",
    "AM2302",
    "T AM2302",
    "T 2302 TIME",
};
static const char data_pin_description[3][20] = {
    "disable",
    "AM2302",
    "cloning",
};

void dcts_init (void) {
    dcts.dcts_id = DCTS_ID_MEASURE;
    strcpy (dcts.dcts_ver, "1.2.0");
    strcpy (dcts.dcts_name, "Parilka");
    strcpy (dcts.dcts_name_cyr, "Парилка");
    dcts.dcts_address = 0x0C;
    dcts.dcts_rtc.day = 1;
    dcts.dcts_rtc.month = 1;
    dcts.dcts_rtc.year = 2000;
    dcts.dcts_rtc.weekday = 6;
    dcts.dcts_rtc.hour = 12;
    dcts.dcts_rtc.minute = 0;
    dcts.dcts_rtc.second = 0;
    dcts.dcts_pwr = 0.0f;
    dcts.dcts_meas_num = MEAS_NUM;
    dcts.dcts_rele_num = RELE_NUM;
    dcts.dcts_act_num  = ACT_NUM;
    dcts.dcts_alrm_num = ALRM_NUM;

    //meas_channels

    dcts_meas_channel_init(TMPR, "Temperature", "Температура", "°C", "°C");
    dcts_meas_channel_init(TMPR_ADC, "Temperature_adc", "Температура АЦП", "adc", "adc");
    dcts_meas_channel_init(TMPR_V, "Temperature_v", "Температура В", "V", "В");
    dcts_meas_channel_init(VREFINT_ADC, "Vref_adc", "ИОН АЦП", "adc", "adc");
    dcts_meas_channel_init(AM2302_T, "AM2302_T", "Температура AM2302", "°C", "°C");
    dcts_meas_channel_init(VREFINT_ADC, "AM2302_H", "Влажность AM2302", "%", "%");
}*/

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void){
    HAL_Init();
    SystemClock_Config();
    tim_us_init();
    //restore_params();
    gpio_init();
    //menu_init();
#if RELEASE
    MX_IWDG_Init();
#endif //RELEASE
    osThreadDef(duty_task, duty_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
    dutyTaskHandle = osThreadCreate(osThread(duty_task), NULL);

#if RTC_EN
    osThreadDef(rtc_task, rtc_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    rtcTaskHandle = osThreadCreate(osThread(rtc_task), NULL);
#endif // RTC_EN
#if DISPLAY_EN
    osThreadDef(display_task, display_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
    displayTaskHandle = osThreadCreate(osThread(display_task), NULL);
#endif // DISPLAY_EN
#if ADC_INT_EN
    osThreadDef(adc_int_task, adc_int_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
    adcTaskHandle = osThreadCreate(osThread(adc_int_task), NULL);
#endif // ADC_INT_EN
#if AM2302_EN
    osThreadDef(am2302_task, am2302_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    am2302TaskHandle = osThreadCreate(osThread(am2302_task), NULL);
#endif // AM2302_EN
#if BUTTONS_EN
    osThreadDef(buttons_task, buttons_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    buttonsTaskHandle = osThreadCreate(osThread(buttons_task), NULL);
    osThreadDef(navigation_task, navigation_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    navigationTaskHandle = osThreadCreate(osThread(navigation_task), NULL);
#endif // BUTTONS_EN
#if MODBUS_RTU_EN
    osThreadDef(uart_task, uart_task, osPriorityHigh, 0, configMINIMAL_STACK_SIZE*4);
    uartTaskHandle = osThreadCreate(osThread(uart_task), NULL);
#endif // MODBUS_RTU_EN
#if DS18B20_EN
    osThreadDef(ds18b20_task, ds18b20_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    am2302TaskHandle = osThreadCreate(osThread(ds18b20_task), NULL);
#endif // DS18B20_EN
    /* Start scheduler */
    osKernelStart();

    while (1)  {

    }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    //RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV128;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the Systick interrupt time
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

void duty_task(void const * argument){
    (void)argument;
    uint32_t task_tick = 0;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        leds_handle();
        if(((task_tick++)%10u)==0u){// every 1 sec
            /* lifebit led on */
            led_os_on(200);
        }
        osDelayUntil(&last_wake_time, DUTY_TASK_PERIOD_MS);
    }
}

static void MX_IWDG_Init(void){

    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
    hiwdg.Init.Reload = 3124;   //10sec
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
      Error_Handler();
    }
}

/**
 * @brief Init us timer
 * @ingroup MAIN
 */
static void tim_us_init(void){
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    __HAL_RCC_TIM2_CLK_ENABLE();
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 71;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFF;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)  {
        _Error_Handler(__FILE__, __LINE__);
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)  {
        _Error_Handler(__FILE__, __LINE__);
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)  {
        _Error_Handler(__FILE__, __LINE__);
    }
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK){
        _Error_Handler(__FILE__, __LINE__);
    }
}


/**
 * @brief Get value from global us timer
 * @return global us timer value
 * @ingroup MAIN
 */
uint32_t us_tim_get_value(void){
    uint32_t value = us_cnt_H + TIM2->CNT;
    return value;
}
/**
 * @brief Us delayy
 * @param us - delau value
 * @ingroup MAIN
 */
void us_tim_delay(uint32_t us){
    uint32_t current;
    uint8_t with_yield;
    current = TIM2->CNT;
    with_yield = 0;
    if(us > TIME_YIELD_THRESHOLD){
        with_yield =1;
    }
    while ((TIM2->CNT - current)<us){
        if(with_yield){
            osThreadYield();
        }
    }
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM1) {
        HAL_IncTick();
    }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

static void gpio_init(void){
    //Enable GPIO's CLK
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    //Init LED PIN's
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = LED_SYS_G_PIN;
    HAL_GPIO_WritePin(LED_SYS_G_PORT, LED_SYS_G_PIN, GPIO_PIN_SET);
    HAL_GPIO_Init (LED_SYS_G_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED_SYS_R_PIN;
    HAL_GPIO_WritePin(LED_SYS_R_PORT, LED_SYS_R_PIN, GPIO_PIN_SET);
    HAL_GPIO_Init (LED_SYS_R_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED_CON_G_PIN;
    HAL_GPIO_WritePin(LED_CON_G_PORT, LED_CON_G_PIN, GPIO_PIN_SET);
    HAL_GPIO_Init (LED_CON_G_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED_CON_R_PIN;
    HAL_GPIO_WritePin(LED_CON_R_PORT, LED_CON_R_PIN, GPIO_PIN_SET);
    HAL_GPIO_Init (LED_CON_R_PORT, &GPIO_InitStruct);

    //Init MDB ADDR PIN's
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = MDB_ADDR_0_PIN;
    HAL_GPIO_Init (MDB_ADDR_0_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_ADDR_1_PIN;
    HAL_GPIO_Init (MDB_ADDR_1_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_ADDR_2_PIN;
    HAL_GPIO_Init (MDB_ADDR_2_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_ADDR_3_PIN;
    HAL_GPIO_Init (MDB_ADDR_3_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_ADDR_4_PIN;
    HAL_GPIO_Init (MDB_ADDR_4_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_ADDR_5_PIN;
    HAL_GPIO_Init (MDB_ADDR_5_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_ADDR_6_PIN;
    HAL_GPIO_Init (MDB_ADDR_6_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_ADDR_7_PIN;
    HAL_GPIO_Init (MDB_ADDR_7_PORT, &GPIO_InitStruct);

    //Init MDB RATE PIN's
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = MDB_RATE_0_PIN;
    HAL_GPIO_Init (MDB_RATE_0_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_RATE_1_PIN;
    HAL_GPIO_Init (MDB_RATE_1_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_RATE_2_PIN;
    HAL_GPIO_Init (MDB_RATE_2_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MDB_RATE_3_PIN;
    HAL_GPIO_Init (MDB_RATE_3_PORT, &GPIO_InitStruct);
}

void refresh_watchdog(void){
#if RELEASE
        HAL_IWDG_Refresh(&hiwdg);
#endif //RELEASE
}


uint32_t uint32_pow(uint16_t x, uint8_t pow){
    uint32_t result = 1;
    while(pow){
        result *= x;
        pow--;
    }
    return result;
}
static int leds_handle(void){
    int res = 0;
    if(led_os_error_on_time ){
        HAL_GPIO_WritePin(LED_SYS_R_PORT, LED_SYS_R_PIN,GPIO_PIN_SET);
        led_os_error_on_time = led_os_error_on_time>100?led_os_error_on_time-100:0;
    }else{
        HAL_GPIO_WritePin(LED_SYS_R_PORT, LED_SYS_R_PIN,GPIO_PIN_RESET);
    }
    if(led_os_on_time ){
        HAL_GPIO_WritePin(LED_SYS_G_PORT, LED_SYS_G_PIN,GPIO_PIN_RESET);
        led_os_on_time = led_os_on_time>100?led_os_on_time-100:0;
    }else{
        HAL_GPIO_WritePin(LED_SYS_G_PORT, LED_SYS_G_PIN,GPIO_PIN_SET);
    }
    if(led_packet_recv_time){
        HAL_GPIO_WritePin(LED_CON_G_PORT, LED_CON_G_PIN,GPIO_PIN_SET);
        led_packet_recv_time = led_packet_recv_time>100?led_packet_recv_time-100:0;
    }else{
        HAL_GPIO_WritePin(LED_CON_G_PORT, LED_CON_G_PIN,GPIO_PIN_RESET);
    }
    if(led_packet_send_time){
        HAL_GPIO_WritePin(LED_CON_R_PORT, LED_CON_R_PIN,GPIO_PIN_SET);
        led_packet_send_time = led_packet_send_time>100?led_packet_send_time-100:0;
    }else{
        HAL_GPIO_WritePin(LED_CON_R_PORT, LED_CON_R_PIN,GPIO_PIN_RESET);
    }
    return res;
}
void led_os_error_on(u16 time_ms){
    if(!led_os_error_on_time){led_os_error_on_time = time_ms;}
    return;
}
void led_os_on(u16 time_ms){
    if(!led_os_on_time){led_os_on_time = time_ms;}
    return;
}

void led_packet_recv_on(u16 time_ms){
    if(!led_packet_recv_time){led_packet_recv_time = time_ms;}
    return;
}
void led_packet_send_on(u16 time_ms){
    if(!led_packet_send_time ){led_packet_send_time = time_ms;}
    return;
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

