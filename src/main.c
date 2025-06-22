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

#include "main.h"

//-------Global variables------

IWDG_HandleTypeDef hiwdg = {0};
osThreadId main_task_handle = {0};

TIM_HandleTypeDef htim3;
osThreadId defaultTaskHandle;
osThreadId displayTaskHandle;
osThreadId adcTaskHandle;
osThreadId am2302TaskHandle;
osThreadId buttonsTaskHandle;
osThreadId navigationTaskHandle;
osThreadId uartTaskHandle;
uint8_t irq_state = IRQ_NONE;

//-------Static variables------

static u32 led_sys_ok_time = 0;

static edit_val_t edit_val = {0};
static navigation_t navigation_style = MENU_NAVIGATION;
saved_to_flash_t config;
static const uart_bitrate_t bitrate_array[14] = {
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

//-------Static functions declaration-----------

static int SystemClock_Config(void);
static void main_IWDG_Init(void);
static void main_gpio_init(void);
static void main_IWDG_refresh(void);
static int  main_leds_handle(u32 call_period);

static void RTC_Init(void);
static void tim2_init(void);
static void save_to_bkp(u8 bkp_num, u8 var);
static void save_float_to_bkp(u8 bkp_num, float var);
static u8 read_bkp(u8 bkp_num);
static float read_float_bkp(u8 bkp_num, u8 sign);
static void led_lin_init(void);
static void data_pin_irq_init(void);
static void save_params(void);
static void restore_params(void);

static void print_main(void);
static void print_menu(void);
static void print_value(u8 tick);

//-------Functions----------

int main(void){

    HAL_Init();
    SystemClock_Config();
    us_tim_init();
    debug_init();
    //dcts_init();
    //restore_params();
    main_gpio_init();
    //menu_init();
#if RELEASE_FLAG
    main_IWDG_Init();
#endif //RELEASE

    osThreadDef(main_task, main_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    main_task_handle = osThreadCreate(osThread(main_task), NULL);
    if(main_task_handle == NULL){
        debug_msg(__func__, DBG_MSG_ERR, "Can't create main_task");
    }

#if MDB_EN
    osThreadDef(modbus_task, modbus_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    modbus_task_handle = osThreadCreate(osThread(modbus_task), NULL);
    if(modbus_task_handle == NULL){
        debug_msg(__func__, DBG_MSG_ERR, "Can't create modbus_task");
    }else{
        service.vars.mdb_state = MODBUS_INIT_TIMEOUT_MS;
        service.vars.mdb_state |= SRV_ST_CREATED;
    }
#endif // MDB_EN

#if RTC_EN
    osThreadDef(rtc_task, rtc_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    rtc_task_handle = osThreadCreate(osThread(rtc_task), NULL);
    if(rtc_task_handle == NULL){
        debug_msg(__func__, DBG_MSG_ERR, "Can't create rtc_task");
    }else{
        service.vars.rtc_state = RTC_INIT_TIMEOUT_MS;
        service.vars.rtc_state |= SRV_ST_CREATED;
    }
#endif // RTC_EN

/*
    osThreadDef(display_task, display_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
    displayTaskHandle = osThreadCreate(osThread(display_task), NULL);

    osThreadDef(adc_task, adc_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
    adcTaskHandle = osThreadCreate(osThread(adc_task), NULL);

    osThreadDef(am2302_task, am2302_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    am2302TaskHandle = osThreadCreate(osThread(am2302_task), NULL);

    osThreadDef(buttons_task, buttons_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    buttonsTaskHandle = osThreadCreate(osThread(buttons_task), NULL);

    osThreadDef(navigation_task, navigation_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    navigationTaskHandle = osThreadCreate(osThread(navigation_task), NULL);

    osThreadDef(uart_task, uart_task, osPriorityHigh, 0, configMINIMAL_STACK_SIZE*4);
    uartTaskHandle = osThreadCreate(osThread(uart_task), NULL);

    osThreadDef(ds18b20_task, ds18b20_task, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    am2302TaskHandle = osThreadCreate(osThread(ds18b20_task), NULL);
*/
    /* Start scheduler */
    osKernelStart();

    while (1)  {

    }

}

void main_task(void const * argument){
    (void)argument;
    uint32_t last_wake_time = osKernelSysTick();
    u32 tick = 0;
    while(1){
        // Every 1 second
        if(((tick)%(1000/MAIN_TASK_PERIOD))==0u){
            // Blink System OK LED
            led_sys_ok_time = 100;

            // Update os.vars.runtime counter
            os.vars.runtime += 1;

            // Call adc_service_meas()
            //adc_service_meas();
        }
        // For debug only
        if(tick == 50){
            debug_msg(__func__, DBG_MSG_ERR, "Test error message, tick = %d", tick);
        }else if(tick == 100){
            debug_msg(__func__, DBG_MSG_INFO, "Test info message");
        }
        // Blinks LEDs control
        main_leds_handle(MAIN_TASK_PERIOD);

        // Refresh IWDG
        main_IWDG_refresh();

        // Checks other tasks state and restart them if error or suspend

        osDelayUntil(&last_wake_time, MAIN_TASK_PERIOD);
        tick++;
        if(tick == MAIN_TASK_TICK_MAX){
            tick = 0;
        }
    }
}

//void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
/*
void dcts_init (void) {
    dcts.dcts_id = DCTS_ID_MEASURE;
    strcpy (dcts.dcts_ver, "1.2.0");
    strcpy (dcts.dcts_name, "Parilka");
    strcpy (dcts.dcts_name_cyr, "???????");
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

    dcts_meas_channel_init(TMPR, "Temperature", "???????????", "?C", "?C");
    dcts_meas_channel_init(TMPR_ADC, "Temperature_adc", "??????????? ???", "adc", "adc");
    dcts_meas_channel_init(TMPR_V, "Temperature_v", "??????????? ?", "V", "?");
    dcts_meas_channel_init(VREFINT_ADC, "Vref_adc", "??? ???", "adc", "adc");
    dcts_meas_channel_init(AM2302_T, "AM2302_T", "??????????? AM2302", "?C", "?C");
    dcts_meas_channel_init(VREFINT_ADC, "AM2302_H", "????????? AM2302", "%", "%");
}
*/



/**
 * @brief display_task
 * @param argument
 */

#define display_task_period 500
#define SHOW_TIME 5
#define PAUSE_DELAY 2
void display_task(void const * argument){
    (void)argument;
    char string[50] = {0};
    char * p_string = string;
    u8 tick = 0;
    //uint16_t color = 1;
    menu_page_t last_page = selectedMenuItem->Page;
    //refresh_watchdog();
    max7219_init();
    //st7735_init();
    sprintf(string, "       dcts%s", dcts.dcts_ver);
    while(*p_string != '\0'){
        max7219_print_string(p_string);
        if(*p_string == 'd'){
            //refresh_watchdog();
            osDelay(2000);
        }else{
            //refresh_watchdog();
            osDelay(100);
        }
        p_string++;
        //refresh_watchdog();
    }
    max7219_clr();
    osDelay(500);

    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        sprintf(string, "%02d-%02d-%02d", dcts.dcts_rtc.hour, dcts.dcts_rtc.minute, dcts.dcts_rtc.second);
        /*ST7735_fill_rect(0,0,160,128,color);
        color++;
        ST7735_fill_rect(50,55,(uint8_t)strlen(string)*Font_7x10.FontWidth+2,Font_7x10.FontHeight+2,ST7735_BLACK);
        st7735_xy(51,55);
        st7735_print(string, &Font_7x10, ST7735_WHITE);*/

        //refresh_watchdog();
        if(last_page != selectedMenuItem->Page){
            tick = 0;
            last_page = selectedMenuItem->Page;
        }
        if(selectedMenuItem->Page == MAIN_PAGE){
            print_main();
        }else if(selectedMenuItem->Child_num > 0){
            print_menu();
        }else if(selectedMenuItem->Child_num == 0){
            if(tick > PAUSE_DELAY){
                print_value(tick - PAUSE_DELAY);
            }else{
                print_value(0);
            }
        }
        if((pressed_time[BUTTON_OK].pressed > RESET_HOLD)&&(pressed_time[BUTTON_BREAK].pressed > RESET_HOLD)){
            // reset
            sprintf(string, "       reset");
            p_string = string;
            while(*p_string != '\0'){
                max7219_print_string(p_string);
                if(*p_string == 'r'){
                    //refresh_watchdog();
                    osDelay(1000);
                }else{
                    //refresh_watchdog();
                    osDelay(100);
                }
                p_string++;
                //refresh_watchdog();
            }
            NVIC_SystemReset();
        }
        tick++;
        osDelayUntil(&last_wake_time, display_task_period);
    }
}

static void print_main(void){
    static u8 tick = 0;
    char string[50] = {0};

    switch (config.params.skin){
    case HIGH_T_AND_TIME:
        if(tick < SHOW_TIME*1000/display_task_period){
            sprintf(string, "%02d-%02d-%02d", dcts.dcts_rtc.hour, dcts.dcts_rtc.minute, dcts.dcts_rtc.second);
        }else if((tick >= SHOW_TIME*1000/display_task_period)&&(tick < SHOW_TIME*2*1000/display_task_period)){
            if(dcts_meas[TMPR].value > 100.0f){
                sprintf(string, " %.1f *C", (double)dcts_meas[TMPR].value);
            }else{
                sprintf(string, "  %.1f *C", (double)dcts_meas[TMPR].value);
            }
        }
        break;
    case HIGH_T_ONLY:
        if(dcts_meas[TMPR].value > 100.0f){
            sprintf(string, " %.1f *C", (double)dcts_meas[TMPR].value);
        }else{
            sprintf(string, "  %.1f *C", (double)dcts_meas[TMPR].value);
        }
        break;
    case TIME_ONLY:
        sprintf(string, "%02d-%02d-%02d", dcts.dcts_rtc.hour, dcts.dcts_rtc.minute, dcts.dcts_rtc.second);
        break;
    case AM2302_T_AND_H:
        if(dcts_meas[AM2302_H].valid){
            sprintf(string, "%.1f*C %fH", (double)dcts_meas[AM2302_T].value, (double)dcts_meas[AM2302_H].value);
        }else{
            sprintf(string, "not conn");
        }
        break;
    case HIGH_T_AND_AM2302_T:
        if(tick < SHOW_TIME*1000/display_task_period){
            sprintf(string, "1 %.1f*C", (double)dcts_meas[TMPR].value);
        }else if((tick >= SHOW_TIME*1000/display_task_period)&&(tick < SHOW_TIME*2*1000/display_task_period)){
            if(dcts_meas[AM2302_H].valid){
                sprintf(string, "2 %.1f*C", (double)dcts_meas[AM2302_T].value);
            }else{
                sprintf(string, "not conn");
            }
        }
        break;
    case HIGH_T_AND_AM2302_T_AND_TIME:
        if(tick < SHOW_TIME*1000/display_task_period){
            sprintf(string, "%02d-%02d-%02d", dcts.dcts_rtc.hour, dcts.dcts_rtc.minute, dcts.dcts_rtc.second);
        }else if(tick < SHOW_TIME*2*1000/display_task_period){
            sprintf(string, "1 %.1f*C", (double)dcts_meas[TMPR].value);
        }else if((tick >= SHOW_TIME*2*1000/display_task_period)&&(tick < SHOW_TIME*3*1000/display_task_period)){
            if(dcts_meas[AM2302_H].valid){
                sprintf(string, "2 %.1f*C", (double)dcts_meas[AM2302_T].value);
            }else{
                sprintf(string, "not conn");
            }
        }
        break;
    }
    tick++;
    switch (config.params.skin) {
    case HIGH_T_AND_AM2302_T_AND_TIME:
        if(tick == SHOW_TIME*3*1000/display_task_period){
            tick = 0;
        }
        break;
    default:
        if(tick == SHOW_TIME*2*1000/display_task_period){
            tick = 0;
        }
    }
    max7219_print_string(string);
}

static void print_menu(void){
    char string[50] = {0};
    sprintf(string, selectedMenuItem->Text);
    max7219_print_string(string);
}

static void print_value(u8 position){
    char string[50] = {0};
    char * p_string = string;
    switch (selectedMenuItem->Page){
    case DCTS_VER:
        sprintf(string, "%s %s",selectedMenuItem->Text, dcts.dcts_ver);
        break;
    case V_PWR:
        sprintf(string, "%s %.2f",selectedMenuItem->Text, (double)dcts.dcts_pwr);
        break;
    case MEAS_CH_0:
        sprintf(string, "%s %.2f",selectedMenuItem->Text, (double)dcts_meas[0].value);
        break;
    case MEAS_CH_1:
        sprintf(string, "%s %.2f",selectedMenuItem->Text, (double)dcts_meas[1].value);
        break;
    case MEAS_CH_2:
        sprintf(string, "%s %.2f",selectedMenuItem->Text, (double)dcts_meas[2].value);
        break;
    case MEAS_CH_3:
        sprintf(string, "%s %.2f",selectedMenuItem->Text, (double)dcts_meas[3].value);
        break;
    case MEAS_CH_4:
        sprintf(string, "%s %.2f",selectedMenuItem->Text, (double)dcts_meas[4].value);
        break;
    case MEAS_CH_5:
        sprintf(string, "%s %.2f",selectedMenuItem->Text, (double)dcts_meas[5].value);
        break;
    case TMPR_COEF_A:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %d",selectedMenuItem->Text, config.params.tmpr_coef_a);
            edit_val.type = VAL_UINT16;
            edit_val.digit_max = 2;
            edit_val.digit = 0;
            edit_val.val_min.uint16 = 0;
            edit_val.val_max.uint16 = 999;
            edit_val.p_val.p_uint16 = &config.params.tmpr_coef_a;
        }else{
            sprintf(string, "       %d",config.params.tmpr_coef_a);
        }
        break;
    case TMPR_COEF_B:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %d",selectedMenuItem->Text, config.params.tmpr_coef_b);
            edit_val.type = VAL_INT16;
            edit_val.digit_max = 1;
            edit_val.digit = 0;
            edit_val.val_min.int16 = -99;
            edit_val.val_max.int16 = 99;
            edit_val.p_val.p_int16 = &config.params.tmpr_coef_b;
        }else{
            sprintf(string, "       %d",config.params.tmpr_coef_b);
        }
        break;
    case MDB_ADDR:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %d",selectedMenuItem->Text, config.params.mdb_address);
            edit_val.type = VAL_UINT16;
            edit_val.digit_max = 1;
            edit_val.digit = 0;
            edit_val.val_min.uint16 = 0;
            edit_val.val_max.uint16 = 99;
            edit_val.p_val.p_uint16 = &config.params.mdb_address;
        }else{
            sprintf(string, "       %d",config.params.mdb_address);
        }
        break;
    case MDB_BITRATE:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %d",selectedMenuItem->Text, bitrate_array[bitrate_array_pointer]*100);
            edit_val.type = VAL_UINT16;
            edit_val.digit_max = 0;
            edit_val.digit = 0;
            edit_val.val_min.uint16 = 0;
            edit_val.val_max.uint16 = 13;
            edit_val.p_val.p_uint16 = &bitrate_array_pointer;
        }else{
            sprintf(string, "       %d",bitrate_array[bitrate_array_pointer]*100);
        }
        config.params.mdb_bitrate = (uint16_t)bitrate_array[bitrate_array_pointer];
        break;
    case MDB_RECIEVED_CNT:
        sprintf(string, "%s %d",selectedMenuItem->Text, uart_2.recieved_cnt);
        break;
    case MDB_SEND_CNT:
        sprintf(string, "%s %d",selectedMenuItem->Text, uart_2.send_cnt);
        break;
    case MDB_OVERRUN_ERR:
        sprintf(string, "%s %d",selectedMenuItem->Text, uart_2.overrun_err_cnt);
        break;
    case MDB_PARITY_ERR:
        sprintf(string, "%s %d",selectedMenuItem->Text, uart_2.parity_err_cnt);
        break;
    case MDB_FRAME_ERR:
        sprintf(string, "%s %d",selectedMenuItem->Text, uart_2.frame_err_cnt);
        break;
    case MDB_NOISE_ERR:
        sprintf(string, "%s %d",selectedMenuItem->Text, uart_2.noise_err_cnt);
        break;
    case LIGHT_LVL:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %d",selectedMenuItem->Text, config.params.light_lvl);
            edit_val.type = VAL_UINT16;
            edit_val.digit_max = 2;
            edit_val.digit = 1;
            edit_val.val_min.uint16 = 10;
            edit_val.val_max.uint16 = 100;
            edit_val.p_val.p_uint16 = &config.params.light_lvl;
        }else{
            sprintf(string, "       %d",config.params.light_lvl);
            if(navigation_style == DIGIT_EDIT){
                max7219_send(0x0A,(u8)(config.params.light_lvl/10));
            }
        }
        break;
    case SKIN:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %s",selectedMenuItem->Text, skin_description[config.params.skin]);
            edit_val.type = VAL_UINT16;
            edit_val.digit_max = 0;
            edit_val.digit = 0;
            edit_val.val_min.uint16 = 0;
            edit_val.val_max.uint16 = SKIN_NMB-1;
            edit_val.p_val.p_uint16 = &config.params.skin;
        }else{
            sprintf(string, "       %s",skin_description[config.params.skin]);
        }
        break;
    case PIN_CONFIG:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %s",selectedMenuItem->Text, data_pin_description[config.params.data_pin_config]);
            edit_val.type = VAL_UINT16;
            edit_val.digit_max = 0;
            edit_val.digit = 0;
            edit_val.val_min.uint16 = 0;
            edit_val.val_max.uint16 = 2;
            edit_val.p_val.p_uint16 = &config.params.data_pin_config;
        }else{
            sprintf(string, "       %s",data_pin_description[config.params.data_pin_config]);
        }
        break;
    case TIME_HOUR:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s  %02d",selectedMenuItem->Text, dcts.dcts_rtc.hour);
            edit_val.type = VAL_UINT8;
            edit_val.digit_max = 1;
            edit_val.digit = 0;
            edit_val.val_min.uint8 = 0;
            edit_val.val_max.uint8 = 23;
            edit_val.p_val.p_uint8 = &dcts.dcts_rtc.hour;
        }else{
            sprintf(string, "      %02d",dcts.dcts_rtc.hour);
        }
        break;
    case TIME_MIN:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %02d",selectedMenuItem->Text, dcts.dcts_rtc.minute);
            edit_val.type = VAL_UINT8;
            edit_val.digit_max = 1;
            edit_val.digit = 0;
            edit_val.val_min.uint8 = 0;
            edit_val.val_max.uint8 = 59;
            edit_val.p_val.p_uint8 = &dcts.dcts_rtc.minute;
        }else{
            sprintf(string, "      %02d",dcts.dcts_rtc.minute);
        }
        break;
    case TIME_SEC:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %02d",selectedMenuItem->Text, dcts.dcts_rtc.second);
            edit_val.type = VAL_UINT8;
            edit_val.digit_max = 1;
            edit_val.digit = 0;
            edit_val.val_min.uint8 = 0;
            edit_val.val_max.uint8 = 59;
            edit_val.p_val.p_uint8 = &dcts.dcts_rtc.second;
        }else{
            sprintf(string, "      %02d",dcts.dcts_rtc.second);
        }
        break;
    case DATE_DAY:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s   %02d",selectedMenuItem->Text, dcts.dcts_rtc.day);
            edit_val.type = VAL_UINT8;
            edit_val.digit_max = 1;
            edit_val.digit = 0;
            edit_val.val_min.uint8 = 1;
            edit_val.val_max.uint8 = 31;
            edit_val.p_val.p_uint8 = &dcts.dcts_rtc.day;
        }else{
            sprintf(string, "      %02d",dcts.dcts_rtc.day);
        }
        break;
    case DATE_MONTH:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %02d",selectedMenuItem->Text, dcts.dcts_rtc.month);
            edit_val.type = VAL_UINT8;
            edit_val.digit_max = 1;
            edit_val.digit = 0;
            edit_val.val_min.uint8 = 1;
            edit_val.val_max.uint8 = 12;
            edit_val.p_val.p_uint8 = &dcts.dcts_rtc.month;
        }else{
            sprintf(string, "      %02d",dcts.dcts_rtc.month);
        }
        break;
    case DATE_YEAR:
        if(navigation_style == MENU_NAVIGATION){
            sprintf(string, "%s %d",selectedMenuItem->Text, dcts.dcts_rtc.year);
            edit_val.type = VAL_UINT16;
            edit_val.digit_max = 3;
            edit_val.digit = 0;
            edit_val.val_min.uint16 = 2000;
            edit_val.val_max.uint16 = 5000;
            edit_val.p_val.p_uint16 = &dcts.dcts_rtc.year;
        }else{
            sprintf(string, "    %d",dcts.dcts_rtc.year);
        }
        break;
    case SAVING:
        sprintf(string, "%s",selectedMenuItem->Text);
        max7219_print_string(string);
        save_params();
        //osDelay(2000);
        menuChange(selectedMenuItem->Parent);
        break;
    default:
        sprintf(string, "not found");
    }

    if(navigation_style == DIGIT_POSITION){
        // add point before edit position
        char string_pos[50] = {0};
        strncpy(string_pos,string,strlen(string) - edit_val.digit - 1);
        strcat(string_pos,".");
        p_string += (strlen(string) - edit_val.digit - 1);
        strcat(string_pos,p_string);
        strcpy(string,string_pos);
        p_string = string;
    }else if(navigation_style == DIGIT_EDIT){
        // blink edited digit
        if(position%2 == 1){
            string[strlen(string) - edit_val.digit - 1] = ' ';
        }
    }

    // remove points len from string
    u8 len = (u8)strlen(string) - (u8)str_smb_num(string, '.');
    if(len > 8){
        if(position < (len-8)){
            p_string += position;
        }else{
            p_string += (len-8);
        }
    }
    max7219_print_string(p_string);
}

/**
 * @brief am2302_task
 * @param argument
 */
void am2302_task (void const * argument){
    (void)argument;
    am2302_init();
    am2302_data_t data_pin;
    uint8_t data_pin_lost_con_cnt = 0;
    uint32_t data_pin_recieved = 0;
    uint32_t data_pin_lost = 0;
    data_pin_irq_init();
    am2302_data_t data = {0};
    //refresh_watchdog();
    osDelay(1000);
    //refresh_watchdog();
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        //refresh_watchdog();
        switch (config.params.data_pin_config){
        case DATA_PIN_EXT_AM2302:
            data_pin = am2302_get(0);
            taskENTER_CRITICAL();
            if(data_pin.error == 1){
                data_pin_lost++;
                data_pin_lost_con_cnt++;
                if(data_pin_lost_con_cnt > 2){
                    dcts_meas[AM2302_H].valid = FALSE;
                    dcts_meas[AM2302_T].valid = FALSE;
                }
            }else{
                data_pin_recieved++;
                data_pin_lost_con_cnt = 0;
                dcts_meas[AM2302_H].value = (float)data_pin.hum/10;
                dcts_meas[AM2302_H].valid = TRUE;
                dcts_meas[AM2302_T].value = (float)data_pin.tmpr/10;
                dcts_meas[AM2302_T].valid = TRUE;
            }
            taskEXIT_CRITICAL();
            osDelayUntil(&last_wake_time, 3000);
            break;
        case DATA_PIN_CLONE_AM2302:
            switch (irq_state) {
            case IRQ_SEND_TMPR:
                data.tmpr = (int16_t)(dcts_meas[TMPR].value * 10.0f);
                data.hum = 0;
                am2302_send(data, 0);
                data_pin_irq_init();
                break;
            case IRQ_READ_RTC:
                data = am2302_get_rtc(0);
                if(data.error != 1){
                    dcts.dcts_rtc.hour = (uint8_t)((data.hum & 0xFF00) >> 8);
                    dcts.dcts_rtc.minute = (uint8_t)(data.hum & 0xFF);
                    dcts.dcts_rtc.second = (uint8_t)((data.tmpr & 0xFF00) >> 8);
                    RTC_set(dcts.dcts_rtc);
                }
                data_pin_irq_init();
                break;
            default:
                taskYIELD();
            }
            break;
        }
    }
}

#define BUTTON_PRESS_TIME 1000
#define BUTTON_PRESS_TIMEOUT 10000
#define BUTTON_CLICK_TIME 10
#define navigation_task_period 20
void navigation_task (void const * argument){
    u16 timeout = 0;
    (void)argument;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        switch (navigation_style){
        case MENU_NAVIGATION:
            if(dcts.dcts_rtc.state == RTC_STATE_EDIT){
                dcts.dcts_rtc.state = RTC_STATE_SET;
            }
            if(button_click(BUTTON_OK,BUTTON_CLICK_TIME)){
                // go to next element
                menuChange(selectedMenuItem->Next);
            }
            if(button_click(BUTTON_BREAK,BUTTON_CLICK_TIME)){
                // go to previous element
                menuChange(selectedMenuItem->Previous);
            }
            if(button_clamp(BUTTON_OK,BUTTON_PRESS_TIME)){
                // go to child
                if(selectedMenuItem->Child == &EDITED_VAL){
                    navigation_style = DIGIT_POSITION;
                }else{
                    menuChange(selectedMenuItem->Child);
                }
                timeout = 0;
                while((pressed_time[BUTTON_OK].last_state == BUTTON_PRESSED)&&(timeout < BUTTON_PRESS_TIMEOUT)){
                    osDelay(1);
                    timeout++;
                }
                pressed_time[BUTTON_OK].pressed = 0;
            }
            if(button_clamp(BUTTON_BREAK,BUTTON_PRESS_TIME)){
                // go to parent
                menuChange(selectedMenuItem->Parent);
                timeout = 0;
                while((pressed_time[BUTTON_BREAK].last_state == BUTTON_PRESSED)&&(timeout < BUTTON_PRESS_TIMEOUT)){
                    osDelay(1);
                    timeout++;
                }
                pressed_time[BUTTON_BREAK].pressed = 0;
            }
            break;
        case DIGIT_POSITION:
            if(button_click(BUTTON_OK,BUTTON_CLICK_TIME)){
                // shift position left
                if(edit_val.digit < edit_val.digit_max){
                    edit_val.digit++;
                }
            }
            if(button_click(BUTTON_BREAK,BUTTON_CLICK_TIME)){
                // shift position right
                if(edit_val.digit > 0){
                    edit_val.digit--;
                }
            }
            if(button_clamp(BUTTON_OK,BUTTON_PRESS_TIME)){
                // enter to DIGIT_EDIT_MODE
                navigation_style = DIGIT_EDIT;
                timeout = 0;
                while((pressed_time[BUTTON_OK].last_state == BUTTON_PRESSED)&&(timeout < BUTTON_PRESS_TIMEOUT)){
                    osDelay(1);
                    timeout++;
                }
                pressed_time[BUTTON_OK].pressed = 0;
            }
            if(button_clamp(BUTTON_BREAK,BUTTON_PRESS_TIME)){
                // exit from DIGIT_POSITION_MODE
                navigation_style = MENU_NAVIGATION;
                timeout = 0;
                while((pressed_time[BUTTON_BREAK].last_state == BUTTON_PRESSED)&&(timeout < BUTTON_PRESS_TIMEOUT)){
                    osDelay(1);
                    timeout++;
                }
                pressed_time[BUTTON_BREAK].pressed = 0;
            }

            break;
        case DIGIT_EDIT:
            switch (selectedMenuItem->Page){
            case TIME_HOUR:
            case TIME_MIN:
            case TIME_SEC:
            case DATE_DAY:
            case DATE_MONTH:
            case DATE_YEAR:
                dcts.dcts_rtc.state = RTC_STATE_EDIT;
                break;
            }
            if(button_click(BUTTON_OK,BUTTON_CLICK_TIME)){
                // increment value
                switch(edit_val.type){
                case VAL_INT8:
                    if(*edit_val.p_val.p_int8 < edit_val.val_max.int8){
                        *edit_val.p_val.p_int8 += (int8_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_int8 > edit_val.val_max.int8)||(*edit_val.p_val.p_int8 < edit_val.val_min.int8)){ //if out of range
                        *edit_val.p_val.p_int8 = edit_val.val_max.int8;
                    }
                    break;
                case VAL_UINT8:
                    if(*edit_val.p_val.p_uint8 < edit_val.val_max.uint8){
                        *edit_val.p_val.p_uint8 += (uint8_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_uint8 > edit_val.val_max.uint8)||(*edit_val.p_val.p_uint8 < edit_val.val_min.uint8)){ //if out of range
                        *edit_val.p_val.p_uint8 = edit_val.val_max.uint8;
                    }
                    break;
                case VAL_INT16:
                    if(*edit_val.p_val.p_int16 < edit_val.val_max.int16){
                        *edit_val.p_val.p_int16 += (int16_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_int16 > edit_val.val_max.int16)||(*edit_val.p_val.p_int16 < edit_val.val_min.int16)){ //if out of range
                        *edit_val.p_val.p_int16 = edit_val.val_max.int16;
                    }
                    break;
                case VAL_UINT16:
                    if(*edit_val.p_val.p_uint16 < edit_val.val_max.uint16){
                        *edit_val.p_val.p_uint16 += (uint16_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_uint16 > edit_val.val_max.uint16)||(*edit_val.p_val.p_uint16 < edit_val.val_min.uint16)){ //if out of range
                        *edit_val.p_val.p_uint16 = edit_val.val_max.uint16;
                    }
                    break;
                case VAL_INT32:
                    if(*edit_val.p_val.p_int32 < edit_val.val_max.int32){
                        *edit_val.p_val.p_int32 += (int32_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_int32 > edit_val.val_max.int32)||(*edit_val.p_val.p_int32 < edit_val.val_min.int32)){ //if out of range
                        *edit_val.p_val.p_int32 = edit_val.val_max.int32;
                    }
                    break;
                case VAL_UINT32:
                    if(*edit_val.p_val.p_uint32 < edit_val.val_max.uint32){
                        *edit_val.p_val.p_uint32 += (uint32_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_uint32 > edit_val.val_max.uint32)||(*edit_val.p_val.p_uint32 < edit_val.val_min.uint32)){ //if out of range
                        *edit_val.p_val.p_uint32 = edit_val.val_max.uint32;
                    }
                    break;
                default:
                    break;
                }
            }
            if(button_click(BUTTON_BREAK,BUTTON_CLICK_TIME)){
                // decrement value
                switch(edit_val.type){
                case VAL_INT8:
                    if(*edit_val.p_val.p_int8 > edit_val.val_min.int8){
                        *edit_val.p_val.p_int8 -= (int8_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_int8 > edit_val.val_max.int8)||(*edit_val.p_val.p_int8 < edit_val.val_min.int8)){ //if out of range
                        *edit_val.p_val.p_int8 = edit_val.val_min.int8;
                    }
                    break;
                case VAL_UINT8:
                    if(*edit_val.p_val.p_uint8 > edit_val.val_min.uint8){
                        *edit_val.p_val.p_uint8 -= (uint8_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_uint8 > edit_val.val_max.uint8)||(*edit_val.p_val.p_uint8 < edit_val.val_min.uint8)){ //if out of range
                        *edit_val.p_val.p_uint8 = edit_val.val_min.uint8;
                    }
                    break;
                case VAL_INT16:
                    if(*edit_val.p_val.p_int16 > edit_val.val_min.int16){
                        *edit_val.p_val.p_int16 -= (int16_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_int16 > edit_val.val_max.int16)||(*edit_val.p_val.p_int16 < edit_val.val_min.int16)){ //if out of range
                        *edit_val.p_val.p_int16 = edit_val.val_min.int16;
                    }
                    break;
                case VAL_UINT16:
                    if(*edit_val.p_val.p_uint16 > edit_val.val_min.uint16){
                        *edit_val.p_val.p_uint16 -= (uint16_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_uint16 > edit_val.val_max.uint16)||(*edit_val.p_val.p_uint16 < edit_val.val_min.uint16)){ //if out of range
                        *edit_val.p_val.p_uint16 = edit_val.val_min.uint16;
                    }
                    break;
                case VAL_INT32:
                    if(*edit_val.p_val.p_int32 > edit_val.val_min.int32){
                        *edit_val.p_val.p_int32 -= (int32_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_int32 > edit_val.val_max.int32)||(*edit_val.p_val.p_int32 < edit_val.val_min.int32)){ //if out of range
                        *edit_val.p_val.p_int32 = edit_val.val_min.int32;
                    }
                    break;
                case VAL_UINT32:
                    if(*edit_val.p_val.p_uint32 > edit_val.val_min.uint32){
                        *edit_val.p_val.p_uint32 -= (uint32_t)uint32_pow(10, edit_val.digit);
                    }
                    if((*edit_val.p_val.p_uint32 > edit_val.val_max.uint32)||(*edit_val.p_val.p_uint32 < edit_val.val_min.uint32)){ //if out of range
                        *edit_val.p_val.p_uint32 = edit_val.val_min.uint32;
                    }
                    break;
                default:
                    break;
                }
            }
            if(button_clamp(BUTTON_OK,BUTTON_PRESS_TIME)){
                // exit from DIGIT_EDOT_MODE
                navigation_style = DIGIT_POSITION;
                timeout = 0;
                while((pressed_time[BUTTON_OK].last_state == BUTTON_PRESSED)&&(timeout < BUTTON_PRESS_TIMEOUT)){
                    osDelay(1);
                    timeout++;
                }
                pressed_time[BUTTON_OK].pressed = 0;
            }
            if(button_clamp(BUTTON_BREAK,BUTTON_PRESS_TIME)){
                // exit from DIGIT_EDOT_MODE
                navigation_style = DIGIT_POSITION;
                timeout = 0;
                while((pressed_time[BUTTON_BREAK].last_state == BUTTON_PRESSED)&&(timeout < BUTTON_PRESS_TIMEOUT)){
                    osDelay(1);
                    timeout++;
                }
                pressed_time[BUTTON_BREAK].pressed = 0;
            }
            break;
        }
        osDelayUntil(&last_wake_time, navigation_task_period);
    }
}

#define uart_task_period 5
void uart_task(void const * argument){
    (void)argument;
    uart_init(config.params.mdb_bitrate, 8, 1, PARITY_NONE, 1000, UART_CONN_LOST_TIMEOUT);
    uint16_t tick = 0;
    char string[100];
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        if((uart_2.state & UART_STATE_RECIEVE)&&\
                ((uint16_t)(us_tim_get_value() - uart_2.timeout_last) > uart_2.timeout)){
            memcpy(uart_2.buff_received, uart_2.buff_in, uart_2.in_ptr);
            uart_2.received_len = uart_2.in_ptr;
            uart_2.in_ptr = 0;
            uart_2.state &= ~UART_STATE_RECIEVE;
            uart_2.state &= ~UART_STATE_ERROR;
            uart_2.state |= UART_STATE_IN_HANDING;
            uart_2.conn_last = 0;
            uart_2.recieved_cnt ++;

            if(modbus_packet_for_me(uart_2.buff_received, uart_2.received_len)){
                memcpy(uart_2.buff_out, uart_2.buff_received, uart_2.received_len);
                uint16_t new_len = modbus_rtu_packet(uart_2.buff_out, uart_2.received_len);
                uart_send(uart_2.buff_out, new_len);
            }
            uart_2.state &= ~UART_STATE_IN_HANDING;
        }
        if(uart_2.conn_last > uart_2.conn_lost_timeout){
            uart_deinit();
            uart_init(config.params.mdb_bitrate, 8, 1, PARITY_NONE, 1000, UART_CONN_LOST_TIMEOUT);
        }
        if(tick == 1000/uart_task_period){
            tick = 0;
            HAL_GPIO_TogglePin(LED_PORT,LED_PIN);
            for(uint8_t i = 0; i < MEAS_NUM; i++){
                sprintf(string, "%s:\t%.1f(%s)\n",dcts_meas[i].name,(double)dcts_meas[i].value,dcts_meas[i].unit);
                if(i == MEAS_NUM - 1){
                    strncat(string,"\n",1);
                }
                //uart_send(string,(uint16_t)strlen(string));
            }
        }else{
            tick++;
            uart_2.conn_last += uart_task_period;
        }

        osDelayUntil(&last_wake_time, uart_task_period);
    }
}

static void data_pin_irq_init(void){
    irq_state = IRQ_NONE;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pin = DATA_PIN;
    HAL_GPIO_Init(DATA_PORT, &GPIO_InitStruct);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}



/**
 * @brief Get number of symbols in string
 * @param string - string for find
 * @param symbol - symbol for find
 * @return number of symbols in string
 */
u16 str_smb_num(char* string, char symbol){
    u16 result = 0;
    char * p_string = string;
    while(1){
        p_string = strchr(p_string, symbol);
        if(p_string){
            result++;
            p_string++;
        }else{
            break;
        }
    }

    return result;
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

static void save_params(void){
    int area_cnt = find_free_area();
    if(area_cnt < 0){
        uint32_t erase_error = 0;
        FLASH_EraseInitTypeDef flash_erase = {0};
        flash_erase.TypeErase = FLASH_TYPEERASE_PAGES;
        flash_erase.NbPages = 1;
        flash_erase.PageAddress = FLASH_SAVE_PAGE_ADDRESS;
        HAL_FLASH_Unlock();
        HAL_FLASHEx_Erase(&flash_erase, &erase_error);
        HAL_FLASH_Lock();
        area_cnt = 0;
    }
    for(uint8_t i = 0; i < SAVED_PARAMS_SIZE; i ++){
        save_to_flash(area_cnt, i, &config.word[i]);
    }
    // rewrite new params
    dcts.dcts_address = (uint8_t)config.params.mdb_address;
    uart_deinit();
    uart_init(config.params.mdb_bitrate, 8, 1, PARITY_NONE, 10000, UART_CONN_LOST_TIMEOUT);
    //delay for show message
    osDelay(2000);
}

static void restore_params(void){
    int area_cnt = find_free_area();
    if(area_cnt != 0){
        if(area_cnt == -1){
            // page is fill, actual values in last area
            area_cnt = SAVE_AREA_NMB - 1;
        }else{
            // set last filled area number
            area_cnt--;
        }
        uint16_t *addr;
        addr = (uint32_t)(FLASH_SAVE_PAGE_ADDRESS + area_cnt*SAVE_AREA_SIZE);
        for(uint8_t i = 0; i < SAVED_PARAMS_SIZE; i++){
            config.word[i] = *addr;
            addr++;
        }
    }else{
        //init default values if saved params not found
        config.params.mdb_address = dcts.dcts_address;
        config.params.mdb_bitrate = BITRATE_115200;
        config.params.light_lvl = 20;
        config.params.skin = HIGH_T_AND_TIME;
        config.params.data_pin_config = DATA_PIN_DISABLE;
        config.params.tmpr_coef_a = 100;
        config.params.tmpr_coef_b = 0;
    }
    for(bitrate_array_pointer = 0; bitrate_array_pointer < 14; bitrate_array_pointer++){
        if(bitrate_array[bitrate_array_pointer] == config.params.mdb_bitrate){
            break;
        }
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
/*
void refresh_watchdog(void){
#if RELEASE
        HAL_IWDG_Refresh(&hiwdg);
#endif //RELEASE
}*/


uint32_t uint32_pow(uint16_t x, uint8_t pow){
    uint32_t result = 1;
    while(pow){
        result *= x;
        pow--;
    }
    return result;
}

//-------Static functions----------

/**
  * @brief System Clock Configuration
  * @ingroup main
  * @return 0 - ok,\n
  *         -1 - HAL_RCC_OscConfig() error,\n
  *         -2 - HAL_RCC_ClockConfig() error,\n
  *         -3 - HAL_SYSTICK_Config() error,\n
  */
static int SystemClock_Config(void){
    int result = 0;
    HAL_StatusTypeDef stat = HAL_OK;

    // Initializes RCC Internal/External Oscillator (HSE, HSI, LSE and LSI) configuration structure definition
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // Use Ext 8MHz Osc

#if RTC_EN
    RCC_OscInitStruct.OscillatorType |= RTC_OSC_TYPE;           // Use RTC Osc
    #if(RTC_OSC_TYPE == RCC_OSCILLATORTYPE_LSE)
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;                    // Enable Ext 32.768kHz RTC Osc
    #elif(RTC_OSC_TYPE == RCC_OSCILLATORTYPE_LSI)
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;                    // Enable Int 40kHz RTC RC
    #endif // RTC_OSC_TYPE
#endif // RTC_EN

    RCC_OscInitStruct.HSEState = RCC_HSE_ON;                    // Enable Ext 8MHz Osc
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;     // Set Ext Osc divider
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                    // Enable Int 8MHz RC
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                // Enable PLL
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;        // Set PLL source = Ext 8MHz Osc
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;                // Set PLLCLK = 8 * 9 = 72MHz

    stat = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if(stat != HAL_OK){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_RCC_OscConfig() %S", hal_status[stat]);
    }

    // Initializes RCC System, AHB and APB busses clock configuration structure definition
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_ClkInitStruct.ClockType |= RCC_CLOCKTYPE_HCLK;          // Config HCLK
    RCC_ClkInitStruct.ClockType |= RCC_CLOCKTYPE_SYSCLK;        // Config SYSCLK
    RCC_ClkInitStruct.ClockType |= RCC_CLOCKTYPE_PCLK1;         // Config PCLK1
    RCC_ClkInitStruct.ClockType |= RCC_CLOCKTYPE_PCLK2;         // Config PCLK2

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;   // Set SYSCLK = PLLCLK = 72MHz
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;          // Set HCLK = SYSCLK = 72MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;           // Set APB1CLK = HCLK/2 = 36MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // Set APB1CLK = HCLK = 72MHz

    stat = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
    if (stat != HAL_OK){
        result = -2;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_RCC_ClockConfig() %S", hal_status[stat]);
    }

    // Configure the Systick interrupt time 1ms
    stat = HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
    if (stat != HAL_OK){
        result = -3;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_SYSTICK_Config() %S", hal_status[stat]);
    }

    // Configure the Systick source
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    // Set SysTick_IRQn interrupt configuration
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

    return result;
}

/**
 * @brief Watchdig timer initialisation
 * @ingroup main
 */
static void main_IWDG_Init(void){
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
    hiwdg.Init.Reload = MAIN_IWDG_PERIOD;
    u32 stat = HAL_IWDG_Init(&hiwdg);
    if (stat != HAL_OK){
        //debug_msg(__func__, DBG_MSG_ERR, "HAL_IWDG_Init() %S", hal_status[stat]);
    }
}

/**
 * @brief Init GPIO common for all modules
 * @ingroup main
 *
 * Init SYS and CON LEDs
 * Init MDB_ADDR switch pins
 * Init MDB_RATE switch pins
 */
static void main_gpio_init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;

    // Init SYS and CON LEDs
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = LED_SYS_R_PIN;
    HAL_GPIO_WritePin(LED_SYS_R_PORT, LED_SYS_R_PIN, GPIO_PIN_RESET);
    HAL_GPIO_Init (LED_SYS_R_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LED_SYS_G_PIN;
    HAL_GPIO_WritePin(LED_SYS_G_PORT, LED_SYS_G_PIN, GPIO_PIN_RESET);
    HAL_GPIO_Init (LED_SYS_G_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LED_CON_R_PIN;
    HAL_GPIO_WritePin(LED_CON_R_PORT, LED_CON_R_PIN, GPIO_PIN_RESET);
    HAL_GPIO_Init (LED_CON_R_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LED_CON_G_PIN;
    HAL_GPIO_WritePin(LED_CON_G_PORT, LED_CON_G_PIN, GPIO_PIN_RESET);
    HAL_GPIO_Init (LED_CON_G_PORT, &GPIO_InitStruct);
}

/**
 * @brief Refresh IWDG
 * @ingroup main
 */
static void main_IWDG_refresh(void){
#if RELEASE_FLAG
        HAL_IWDG_Refresh(&hiwdg);
#endif //RELEASE_FLAG

}

/**
 * @brief LEDs control
 * @param call_period - function calling period in ms
 * @ingroup main
 * @return  0
 *
 * 1. Handle System OK LED (GREEN)
 * 2. Handle System ERROR LED (RED)
 * 3. Handle interfaces OK LED (GREEN)
 * 3. Handle interfaces ERROR LED (RED)
 */
static int main_leds_handle(u32 call_period){
    int result = 0;
    // Handle System OK LED (GREEN)
    if(led_sys_ok_time){
        HAL_GPIO_WritePin(LED_SYS_G_PORT, LED_SYS_G_PIN, GPIO_PIN_SET);
        if(led_sys_ok_time > call_period){
            led_sys_ok_time -= call_period;
        }else{
            led_sys_ok_time = 0;
        }
    }else{
        HAL_GPIO_WritePin(LED_SYS_G_PORT, LED_SYS_G_PIN, GPIO_PIN_RESET);
    }

    // Handle System ERROR LED (RED)
    if(debug_led_err_on_time){
        HAL_GPIO_WritePin(LED_SYS_R_PORT, LED_SYS_R_PIN, GPIO_PIN_SET);
        if(debug_led_err_on_time > call_period){
            debug_led_err_on_time -= call_period;
        }else{
            debug_led_err_on_time = 0;
        }
    }else{
        HAL_GPIO_WritePin(LED_SYS_R_PORT, LED_SYS_R_PIN, GPIO_PIN_RESET);
    }

    // Handle interfaces OK LED (GREEN)
#if MDB_EN
    if(modbus_led_ok_on_time){
        HAL_GPIO_WritePin(LED_CON_G_PORT, LED_CON_G_PIN, GPIO_PIN_SET);
        if(modbus_led_ok_on_time > call_period){
            modbus_led_ok_on_time -= call_period;
        }else{
            modbus_led_ok_on_time = 0;
        }
    }else{
        HAL_GPIO_WritePin(LED_CON_G_PORT, LED_CON_G_PIN, GPIO_PIN_RESET);
    }
#endif // MDB_EN
#if CAN_EN
    if(can_led_ok_on_time){
        HAL_GPIO_WritePin(LED_CON_G_PORT, LED_CON_G_PIN, GPIO_PIN_SET);
        if(can_led_ok_on_time > call_period){
            can_led_ok_on_time -= call_period;
        }else{
            can_led_ok_on_time = 0;
        }
    }else{
        HAL_GPIO_WritePin(LED_CON_G_PORT, LED_CON_G_PIN, GPIO_PIN_RESET);
    }
#endif // CAN_EN

    // Handle interfaces ERROR LED (RED)
#if MDB_EN
    if(modbus_led_err_on_time){
        HAL_GPIO_WritePin(LED_CON_R_PORT, LED_CON_R_PIN, GPIO_PIN_SET);
        if(modbus_led_err_on_time > call_period){
            modbus_led_err_on_time -= call_period;
        }else{
            modbus_led_err_on_time = 0;
        }
    }else{
        HAL_GPIO_WritePin(LED_CON_R_PORT, LED_CON_R_PIN, GPIO_PIN_RESET);
    }
#endif // MDB_EN
#if CAN_EN
    if(can_led_err_on_time){
        HAL_GPIO_WritePin(LED_CON_R_PORT, LED_CON_R_PIN, GPIO_PIN_SET);
        if(can_led_err_on_time > call_period){
            can_led_err_on_time -= call_period;
        }else{
            can_led_err_on_time = 0;
        }
    }else{
        HAL_GPIO_WritePin(LED_CON_R_PORT, LED_CON_R_PIN, GPIO_PIN_RESET);
    }
#endif // CAN_EN
    return result;
}
