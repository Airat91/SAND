#include "sand_config.h"
#if BUTTONS_EN

#include "buttons.h"


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
/**
  * @defgroup button
  * @brief work with buttons
  */

/**
  * @addtogroup button
  * @{
  */
#define BUTTONS_TASK_PERIOD 1
#define BUTTONS_NUM 2
/**
  * @}
  */

/**
 * @brief var for read buttons pressed time
 * @ingroup button
 */
button_t pressed_time[BUTTONS_NUM];

static void buttons_init(void);
/**
 * @brief Buttons state read task
 * @param argument - None
 * @ingroup button
 *
 * Increments button time while pressed and save it value after release
 */
void buttons_task (void const * argument){
    (void) argument;
    buttons_init();
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        for(u8 button = 0; button < BUTTONS_NUM; button++){
            if(!HAL_GPIO_ReadPin(pressed_time[button].port, pressed_time[button].pin)){
                if(pressed_time[button].last_state == BUTTON_RELEASE){
                    pressed_time[button].pressed = 0;
                }
                pressed_time[button].pressed += BUTTONS_TASK_PERIOD;
                pressed_time[button].last_state = BUTTON_PRESSED;
            }else{
                pressed_time[button].last_state = BUTTON_RELEASE;
                //pressed_time[button].pressed = 0;
            }
        }
        osDelayUntil(&last_wake_time, BUTTONS_TASK_PERIOD);
    }
}
/**
 * @brief Init buttons GPIOs and global pressed_time
 * @ingroup button
 */
static void buttons_init(void){
    /* pressed_time init */
    pressed_time[BUTTON_OK].pin     = BUTTON_1_PIN;
    pressed_time[BUTTON_OK].port    = BUTTON_1_PORT;
    pressed_time[BUTTON_BREAK].pin  = BUTTON_2_PIN;
    pressed_time[BUTTON_BREAK].port = BUTTON_2_PORT;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Buttons */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    for(u8 i = 0; i < BUTTONS_NUM; i++){
        GPIO_InitStruct.Pin = pressed_time[i].pin;
        HAL_GPIO_Init(pressed_time[i].port, &GPIO_InitStruct);
        pressed_time[i].pressed = 0;
        pressed_time[i].last_state = BUTTON_RELEASE;
    }
}
/**
 * @brief Checks how long button was pressed before last release
 * @param button - button from @ref button_list_t
 * @param time - pressed time in ms
 * @return  0 - button was pressed less than time,\n
 *          1 - button was pressed more than time
 * @ingroup button
 * @warning Resets pressed_time if return 1
 */
uint8_t button_click(button_list_t button, uint16_t time){
    uint8_t result = 0;
    if((pressed_time[button].pressed >= time)&&
            (HAL_GPIO_ReadPin(pressed_time[button].port, pressed_time[button].pin))){
        result = 1;
        pressed_time[button].pressed = 0;
    }
    return result;
}
/**
 * @brief Checks how long button is pressed now
 * @param button - button from @ref button_list_t
 * @param time - pressed time in ms
 * @return  0 - button is pressed less than time,\n
 *          1 - button is pressed more than time
 * @ingroup button
 */
uint8_t button_clamp(button_list_t button, uint16_t time){
    uint8_t result = 0;
    if((pressed_time[button].pressed >= time)&&
            (!HAL_GPIO_ReadPin(pressed_time[button].port, pressed_time[button].pin))){
        result = 1;
    }
    return result;
}

#endif // BUTTONS_EN
