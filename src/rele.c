/*
 * File:        rele.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Work with RELE channels
 * Revision history: 0.1
 */

#if(DEVICE_TYPE == BRIOM_1RELE)

#include "rele.h"

//-------Global variables------

rele_pcb_t rele_pcb = {0};      // RELE process control block

//-------Static variables------

static const rele_gpio_t rele_gpio[RELE_CH_NUM] = {
    {.port = RELE_MCU_1_PORT,.pin = RELE_MCU_1_PIN},
};

//-------Static functions declaration-----------

static int rele_gpio_init (void);
static int rele_gpio_deinit (void);

//-------Functions----------


int rele_init(rele_pcb_t* rele_pcb){
    int result = 0;

    // Init GPIO
    rele_gpio_init();
    // Write module version to vars
    u16 temp[2] = RELE_MODULE_VER;
    memcpy(rele.vars.rele_version, temp, 2*2);
    // Init RELE proccess control block
    memset(rele_pcb, 0, sizeof(rele_pcb_t));
    for(u16 i = 0; i < RELE_CH_NUM; i++){
        rele_pcb->channel[i].state = RELE_ST_OFF;
        rele_pcb->channel[i].last_state = RELE_ST_OFF;
        rele_pcb->channel[i].last_change = 0;
    }

    debug_msg(__func__, DBG_MSG_INFO, "RELE inited");

    return result;
}

int rele_deinit(rele_pcb_t* rele_pcb){
    int result = 0;

    rele_gpio_deinit();

    return result;
}

int rele_handle(rele_pcb_t* rele_pcb, u16 period_ms){
    int result = 0;

    u32 cur_time = us_tim_get_value()/1000; // Get current time in ms

    if(rele_pcb->channel[0].state != rele.vars.rele_ctrl){
        // Check toggle timeout
        if(cur_time > (rele_pcb->channel[0].last_change + RELE_TOGGLE_TIMEOUT_MS)){
            //Toggle RELE GPIO
            if(rele.vars.rele_ctrl == 0){
                HAL_GPIO_WritePin(rele_gpio[0].port, rele_gpio[0].pin, GPIO_PIN_RESET);
                rele_pcb->channel[0].state = RELE_ST_OFF;
            }else{
                HAL_GPIO_WritePin(rele_gpio[0].port, rele_gpio[0].pin, GPIO_PIN_SET);
                rele_pcb->channel[0].state = RELE_ST_ON;
                // Increase rele counter
                rele.vars.rele_cnt++;
            }
            // Update last_change time
            rele_pcb->channel[0].last_change = cur_time;
        }
    }

    // Update rele total counter
    rele.vars.rele_cnt_total = rele.vars.rele_cnt_storage + rele.vars.rele_cnt;

    return result;
}


int rele_save_before_reset(void){
    int result = 0;

    rele.vars.rele_cnt_storage += rele.vars.rele_cnt;

    return result;
}


//-------Static functions----------

/**
 * @brief Init RELE GPIO's
 * @ingroup rele
 * @return 0
 *
 */
static int rele_gpio_init (void){
    int result = 0;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;

    // Init RELE pins
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    for(u16 i = 0; i < RELE_CH_NUM; i++){
        HAL_GPIO_WritePin(rele_gpio->port, rele_gpio->pin, GPIO_PIN_RESET);
        GPIO_InitStruct.Pin = rele_gpio->pin;
        HAL_GPIO_Init (rele_gpio->port, &GPIO_InitStruct);
    }

    return result;
}

/**
 * @brief Deinit RELE GPIO's
 * @ingroup rele
 * @return 0
 *
 */
static int rele_gpio_deinit (void){
    int result = 0;

    for(u16 i = 0; i < RELE_CH_NUM; i++){
        HAL_GPIO_DeInit(rele_gpio->port, rele_gpio->pin);
    }

    return result;
}

#endif // DEVICE_TYPE
