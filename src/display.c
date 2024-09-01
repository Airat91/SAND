#include "sand_config.h"
#if DISPLAY_EN

#include "display.h"

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
    refresh_watchdog();
    max7219_init();
    //st7735_init();
    sprintf(string, "       dcts%s", dcts.dcts_ver);
    while(*p_string != '\0'){
        max7219_print_string(p_string);
        if(*p_string == 'd'){
            refresh_watchdog();
            osDelay(2000);
        }else{
            refresh_watchdog();
            osDelay(100);
        }
        p_string++;
        refresh_watchdog();
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

        refresh_watchdog();
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
                    refresh_watchdog();
                    osDelay(1000);
                }else{
                    refresh_watchdog();
                    osDelay(100);
                }
                p_string++;
                refresh_watchdog();
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


#endif // DISPLAY_EN
