/*
 * File:        ai.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Work with AI channels based on internal ADC
 * Revision history: 0.2
 *      0.2 - Added pseudo buffering algorithm
 */

#include "ai.h"

//-------Global variables------

osThreadId ai_task_handle = {0};
ai_pcb_t ai_pcb = {0};              // Internal ADC process control block

//-------Static variables------

//-------Static functions declaration-----------

static int ai_gpio_init (void);
static int ai_gpio_deinit (void);
static int ai_adc_init(ai_pcb_t* ai_pcb);
static int ai_adc_deinit(ai_pcb_t* ai_pcb);
static int ai_handle_results(ai_pcb_t* ai_pcb);

//-------Functions----------

void ai_task(void const * argument){
    (void)argument;

    if(ai_init(&ai_pcb) == 0){
        service.vars.ai_state |= SRV_ST_RUN;
        debug_msg(__func__, DBG_MSG_INFO, "AI_task started");
    }
    uint8_t tick = 0;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        if(ai_pcb.state & AI_ST_ERROR){
            // Reinit AI if error
            service.vars.ai_state |= SRV_ST_ERROR;
            debug_msg(__func__, DBG_MSG_ERR, "AI reinit");
            ai_deinit(&ai_pcb);
            osDelay(1000);
            if(ai_init(&ai_pcb) == 0){
                service.vars.ai_state &= ~(u32)SRV_ST_ERROR;
            }
        }else{
            // Normal work process

            // Read ADC results from previous cycle and write them into regs
            ai_handle_results(&ai_pcb);
            // Start convertion cycle (all channels in a row)
            ai_pcb.cycle_done = 0;
            HAL_ADC_Start_IT(&ai_pcb.hadc);

            // Check error counter
            if(ai_pcb.err_cnt > AI_MAX_ERR_NMB){
                ai_pcb.state |= AI_ST_ERROR;
            }
        }

        tick++;
        osDelayUntil(&last_wake_time, AI_TASK_PERIOD);
    }
}

int ai_init (ai_pcb_t* ai_pcb){
    int result = 0;
    HAL_StatusTypeDef stat = HAL_OK;
    // Reset ADC process control block
    memset(ai_pcb, 0, sizeof(ai_pcb_t));
    // Config ADC process control block
    ai_pcb->state = AI_ST_READY;
    ai_pcb->hadc.Instance = _AI_ADC;
    ai_pcb->err_cnt = 0;
    ai_pcb->cur_channel = 0;
    // MCU_AI channels
    ai_pcb->adc_inp[0] = _AI_CHANNEL_0;
    ai_pcb->adc_inp[1] = _AI_CHANNEL_1;

    if(ai_gpio_init() != 0){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "ai_gpio_init() error");
    }else{
        if(ai_adc_init(ai_pcb) != 0){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "ai_adc_init() error");
        }else{
            HAL_NVIC_SetPriority(_AI_ADC_IRQn, AI_PRIO, AI_SUBPRIO);
            HAL_NVIC_EnableIRQ(_AI_ADC_IRQn);
        }
    }

    return result;
}

int ai_deinit (ai_pcb_t* ai_pcb){
    int result = 0;

    // Disable rInterrupt
    HAL_NVIC_DisableIRQ(_AI_ADC_IRQn);
    HAL_ADC_Stop_IT(&ai_pcb->hadc);
    ai_gpio_deinit();
    ai_adc_deinit(ai_pcb);
    // Reset ADC process control block
    memset(ai_pcb, 0, sizeof(ai_pcb_t));

    return result;
}

int ai_irq_callback(ai_pcb_t* ai_pcb){
    int result = 0;

    static ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Rank = 1;
    sConfig.SamplingTime = _AI_ADC_SAMPLETIME;

    HAL_ADC_Stop_IT(&ai_pcb->hadc);
    // Get local vars
    u32 cur_time = us_tim_get_value();
    u16 adc_value = (u16)ai_pcb->hadc.Instance->DR;
    u16 channel = ai_pcb->cur_channel;
#if(AI_AVG_BUF_EN == 1)
    u16 sample_ptr = ai_pcb->sample[ai_pcb->cur_channel].sample_ptr;
    //Read ADC result to sample
    ai_pcb->sample[channel].value[sample_ptr] = adc_value;
    ai_pcb->sample[channel].period[sample_ptr] = cur_time - ai_pcb->sample[channel].last_time;
    //Increase sample_ptr
    ai_pcb->sample[channel].sample_ptr++;
    if(ai_pcb->sample[channel].sample_ptr >= ai.vars.avg_num[channel]){
        ai_pcb->sample[channel].sample_ptr = 0;
    }
#endif // AI_AVG_BUF_EN
    //Increase sample avg divider
    if(ai_pcb->sample[channel].sample_avg_divider < ai.vars.avg_num[channel]){
        ai_pcb->sample[channel].sample_avg_divider++;
    }
    //Increase channel
    ai_pcb->cur_channel++;
    if(ai_pcb->cur_channel >= AI_CH_NUM){
        ai_pcb->cur_channel = 0;
        ai_pcb->cycle_done = 1;
    }
    sConfig.Channel = ai_pcb->adc_inp[ai_pcb->cur_channel];
    HAL_ADC_ConfigChannel(&ai_pcb->hadc, &sConfig);
    //Calc average value
#if(AI_AVG_BUF_EN == 1)
    u32 val_sum = 0;
    u32 time_sum = 0;
    for(u16 i = 0; i < ai.vars.avg_num[channel]; i++){
        val_sum += ai_pcb->sample[channel].value[i];
        time_sum += ai_pcb->sample[channel].period[i];
    }
    //Write values to registers
    ai_pcb->sample[channel].value_avg = (float)val_sum/ai_pcb->sample[channel].sample_avg_divider;
    ai_pcb->sample[channel].sample_rate = (float)ai_pcb->sample[channel].sample_avg_divider/time_sum*1000000.0f;
#else
    float avg_val = 0.0f;
    float avg_sample_rate = 0.0f;
    u32 sample_period = cur_time - ai_pcb->sample[channel].last_time;
    float sample_rate = 1000000.0f/sample_period;

    avg_val = ai_pcb->sample[channel].value_avg * (ai_pcb->sample[channel].sample_avg_divider - 1);
    avg_val += (float)adc_value;
    avg_val = avg_val/ai_pcb->sample[channel].sample_avg_divider;

    avg_sample_rate = ai_pcb->sample[channel].sample_rate * (ai_pcb->sample[channel].sample_avg_divider - 1);
    avg_sample_rate += (float)sample_rate;
    avg_sample_rate = avg_sample_rate/ai_pcb->sample[channel].sample_avg_divider;

    //Write values to registers
    ai_pcb->sample[channel].value_avg = avg_val;
    ai_pcb->sample[channel].sample_rate = avg_sample_rate;
#endif // AI_AVG_BUF_EN
    //Save cur_time in last_time
    ai_pcb->sample[channel].last_time = cur_time;
    if(ai_pcb->cycle_done == 0){
        // Run ADC convertion if cycle not done
        HAL_ADC_Start_IT(&ai_pcb->hadc);
    }

    return result;
}

//-------Static functions----------

/**
 * @brief Init ADC GPIO's
 * @ingroup ai
 * @return 0
 */
static int ai_gpio_init (void){
    int result = 0;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;

    GPIO_InitStruct.Pin = AI_MCU_0_PIN;
    HAL_GPIO_Init(AI_MCU_0_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AI_MCU_1_PIN;
    HAL_GPIO_Init(AI_MCU_1_PORT, &GPIO_InitStruct);

    return result;
}

/**
 * @brief Deinit ADC GPIO's
 * @ingroup ai
 * @return 0
 */
static int ai_gpio_deinit (void){
    int result = 0;

    HAL_GPIO_DeInit(AI_MCU_0_PORT, AI_MCU_0_PIN);
    HAL_GPIO_DeInit(AI_MCU_1_PORT, AI_MCU_1_PIN);

    return result;
}

/**
 * @brief Init ADC
 * @param ai_pcb - pointer to ADC process control block
 * @ingroup ai
 * @return  0 - ok,\n
 *          -1 - HAL_ADC_Init() error,\n
 *          -2 - HAL_ADCEx_Calibration_Start() error,\n
 */
static int ai_adc_init(ai_pcb_t* ai_pcb){
    int result = 0;
    HAL_StatusTypeDef stat = HAL_OK;

    _AI_ADC_CLK_EN();

    //Common config
    ai_pcb->hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    ai_pcb->hadc.Init.ContinuousConvMode = ENABLE;
    ai_pcb->hadc.Init.DiscontinuousConvMode = DISABLE;
    ai_pcb->hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    ai_pcb->hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    ai_pcb->hadc.Init.NbrOfConversion = 1;

    stat = HAL_ADC_Init(&ai_pcb->hadc);
    if (stat != HAL_OK){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_ADC_Init() %S", hal_status[stat]);
    }

    // Config first channel
    static ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ai_pcb->adc_inp[ai_pcb->cur_channel];
    sConfig.Rank = 1;
    sConfig.SamplingTime = _AI_ADC_SAMPLETIME;

    //Self-calibration
    stat = HAL_ADCEx_Calibration_Start(&ai_pcb->hadc);
    if(stat != HAL_OK){
        result = -2;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_ADCEx_Calibration_Start() %S", hal_status[stat]);
    }


    return result;
}

/**
 * @brief Deinit ADC
 * @param ai_pcb - pointer to ADC process control block
 * @ingroup ai
 * @return  0
 */
static int ai_adc_deinit(ai_pcb_t* ai_pcb){
    int result = 0;

    HAL_ADC_DeInit(&ai_pcb->hadc);
    _AI_ADC_CLK_DIS();

    return result;
}

static int ai_handle_results(ai_pcb_t* ai_pcb){
    int result = 0;
    float value = 0.0f;

    if(adc_int_vref_code_avg == NULL){
        result = -1;
    }else{
        for(u16 i = 0; i < AI_CH_NUM; i++){
            value = ADC_INT_VREF_VALUE * ai_pcb->sample[i].value_avg / *adc_int_vref_code_avg *
                    ai.vars.ai_calib_a[i] + ai.vars.ai_calib_b[i];
            ai.vars.ai_value[i] = value;
            ai.vars.ai_adc[i] = (u16)ai_pcb->sample[i].value_avg;
            ai.vars.ai_sample_rate[i] = ai_pcb->sample[i].sample_rate;
            // Engineer units
            ai.vars.meas_value[i] = value * ai.vars.meas_calib_a[i] + ai.vars.meas_calib_b[i];
        }
    }
    return result;
}
