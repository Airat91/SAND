/*
 * File:        adc_int.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Work with internal ADC
 * Revision history: 0.1
 */

#include "adc_int.h"

//-------Global variables------

osThreadId adc_int_task_handle = {0};
adc_int_pcb_t adc_int_pcb = {0};        // Internal ADC process control block
float* adc_int_vref_code_avg = NULL;    // Pointer to voltage reference averaged code

//-------Static variables------

//-------Static functions declaration-----------

static int adc_int_gpio_init (void);
static int adc_int_gpio_deinit (void);
static int adc_int_adc_init(adc_int_pcb_t* adc_int_pcb);
static int adc_int_adc_deinit(adc_int_pcb_t* adc_int_pcb);
static int adc_int_handle_results(adc_int_pcb_t* adc_int_pcb);
static float adc_int_get_result_avg(adc_int_pcb_t* adc_int_pcb, adc_int_channel_t channel);
static float adc_int_get_sample_rate(adc_int_pcb_t* adc_int_pcb, adc_int_channel_t channel);

//-------Functions----------

void adc_int_task(void const * argument){
    (void)argument;

    if(adc_int_init(&adc_int_pcb) == 0){
        service.vars.adc_int_state |= SRV_ST_RUN;
        debug_msg(__func__, DBG_MSG_INFO, "ADC_INT_task started");
    }
    uint8_t tick = 0;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        if(adc_int_pcb.state & ADC_INT_ST_ERROR){
            // Reinit ADC_INT if error
            service.vars.adc_int_state |= SRV_ST_ERROR;
            debug_msg(__func__, DBG_MSG_ERR, "ADC_int reinit");
            adc_int_deinit(&adc_int_pcb);
            osDelay(1000);
            if(adc_int_init(&adc_int_pcb) == 0){
                service.vars.adc_int_state &= ~(u32)SRV_ST_ERROR;
            }
        }else{
            // Normal work process

            // Read ADC results from previous cycle and write them into regs
            adc_int_handle_results(&adc_int_pcb);
            // Start convertion cycle (all channels in a row)
            adc_int_pcb.cycle_done = 0;
            HAL_ADC_Start_IT(&adc_int_pcb.hadc);

            // Check error counter
            if(adc_int_pcb.err_cnt > ADC_INT_MAX_ERR_NMB){
                adc_int_pcb.state |= ADC_INT_ST_ERROR;
            }
        }

        tick++;
        osDelayUntil(&last_wake_time, ADC_INT_TASK_PERIOD);
    }
}

int adc_int_init (adc_int_pcb_t* adc_int_pcb){
    int result = 0;
    HAL_StatusTypeDef stat = HAL_OK;
    u16 ptr = 0;
    // Reset ADC process control block
    memset(adc_int_pcb, 0, sizeof(adc_int_pcb_t));
    // Config ADC process control block
    adc_int_pcb->state = ADC_INT_ST_READY;
    adc_int_pcb->hadc.Instance = _ADC_INT;
    adc_int_pcb->err_cnt = 0;
    adc_int_pcb->cur_channel = 0;
    ptr = 0;
#if(ADC_INT_PWR_EN == 1)
    adc_int_pcb->adc_inp[ptr] = _ADC_CHANNEL_PWR;
    adc_int_pcb->adc_channel[ptr++] = ADC_INT_CH_PWR;
#endif // ADC_INT_PWR_EN

#if(ADC_INT_BAT_EN == 1)
    adc_int_pcb->adc_inp[ptr] = _ADC_CHANNEL_BAT;
    adc_int_pcb->adc_channel[ptr++] = ADC_INT_CH_BAT;
#endif // ADC_INT_BAT_EN

#if(ADC_INT_TEMP_EN == 1)
    adc_int_pcb->adc_inp[ptr] = _ADC_CHANNEL_TEMP;
    adc_int_pcb->adc_channel[ptr++] = ADC_INT_CH_TEMP;
#endif // ADC_INT_TEMP_EN

#if(ADC_INT_VREF_INT_EN == 1)
    #if(ADC_INT_VREF_USE == ADC_INT_VREF_INT)
    // set pointer to VREF code
    adc_int_vref_code_avg = &adc_int_pcb->sample[ptr].value_avg;
    #endif // ADC_INT_VREF_USE
    adc_int_pcb->adc_inp[ptr] = _ADC_CHANNEL_VREF_INT;
    adc_int_pcb->adc_channel[ptr++] = ADC_INT_CH_VREF_INT;
#endif // ADC_INT_VREF_INT_EN

#if(ADC_INT_VREF_EXT_EN == 1)
    #if(ADC_INT_VREF_USE == ADC_INT_VREF_EXT)
    // set pointer to VREF code
    adc_int_vref_code_avg = &adc_int_pcb->sample[ptr].value_avg;
    #endif // ADC_INT_VREF_USE
    adc_int_pcb->adc_inp[ptr] = _ADC_CHANNEL_VREF_EXT;
    adc_int_pcb->adc_channel[ptr++] = ADC_INT_CH_VREF_EXT;
#endif // ADC_INT_VREF_EXT_EN

    if(adc_int_gpio_init() != 0){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "adc_int_gpio_init() error");
    }else{
        if(adc_int_adc_init(adc_int_pcb) != 0){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "adc_int_adc_init() error");
        }else{
            HAL_NVIC_SetPriority(_ADC_INT_IRQn, ADC_INT_PRIO, ADC_INT_SUBPRIO);
            HAL_NVIC_EnableIRQ(_ADC_INT_IRQn);
        }
    }

    return result;
}

int adc_int_deinit (adc_int_pcb_t* adc_int_pcb){
    int result = 0;

    // Disable rInterrupt
    HAL_NVIC_DisableIRQ(_ADC_INT_IRQn);
    HAL_ADC_Stop_IT(&adc_int_pcb->hadc);
    adc_int_gpio_deinit();
    adc_int_adc_deinit(adc_int_pcb);
    // Reset ADC process control block
    memset(adc_int_pcb, 0, sizeof(adc_int_pcb_t));

    return result;
}

int adc_int_irq_callback(adc_int_pcb_t* adc_int_pcb){
    int result = 0;
    static u16 bat_meas_cnt = 0;

    static ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

    HAL_ADC_Stop_IT(&adc_int_pcb->hadc);
    // Get local vars
    u32 time = us_tim_get_value();
    u16 sample_ptr = adc_int_pcb->sample[adc_int_pcb->cur_channel].sample_ptr;
    u16 channel = adc_int_pcb->cur_channel;
    //Read ADC result to sample
    adc_int_pcb->sample[channel].value[sample_ptr] = (u16)adc_int_pcb->hadc.Instance->DR;
    adc_int_pcb->sample[channel].period[sample_ptr] = time - adc_int_pcb->sample[channel].last_time;
    adc_int_pcb->sample[channel].last_time = time;
    //Increase sample_ptr
    adc_int_pcb->sample[channel].sample_ptr++;
    if(adc_int_pcb->sample[channel].sample_ptr >= ADC_INT_SAMPLE_NUM){
        adc_int_pcb->sample[channel].sample_ptr = 0;
    }
    //Increase sample avg divider
    if(adc_int_pcb->sample[channel].sample_avg_divider < ADC_INT_SAMPLE_NUM){
        adc_int_pcb->sample[channel].sample_avg_divider++;
    }
    //Increase channel
    adc_int_pcb->cur_channel++;
    if(adc_int_pcb->adc_channel[adc_int_pcb->cur_channel] == ADC_INT_CH_BAT){
        if(bat_meas_cnt != 0){
            //Skip battery channel
            adc_int_pcb->cur_channel++;
        }
        bat_meas_cnt++;
        if(bat_meas_cnt * ADC_INT_TASK_PERIOD > ADC_INT_BAT_PERIOD_US){
            bat_meas_cnt = 0;
        }
    }
    //Special handle for battery channel
    if(adc_int_pcb->cur_channel >= ADC_INT_CH_NUM){
        adc_int_pcb->cur_channel = 0;
        adc_int_pcb->cycle_done = 1;
    }
    sConfig.Channel = adc_int_pcb->adc_inp[adc_int_pcb->cur_channel];
    HAL_ADC_ConfigChannel(&adc_int_pcb->hadc, &sConfig);
    //Calc average value
    u32 val_sum = 0;
    u32 time_sum = 0;
    for(u16 i = 0; i < ADC_INT_SAMPLE_NUM; i++){
        val_sum += adc_int_pcb->sample[channel].value[i];
        time_sum += adc_int_pcb->sample[channel].period[i];
    }
    adc_int_pcb->sample[channel].value_avg = (float)val_sum/adc_int_pcb->sample[channel].sample_avg_divider;
    adc_int_pcb->sample[channel].sample_rate = (float)adc_int_pcb->sample[channel].sample_avg_divider/time_sum*1000000.0f;

    if(adc_int_pcb->cycle_done == 0){
        // Run ADC convertion if cycle not done
        HAL_ADC_Start_IT(&adc_int_pcb->hadc);
    }

    return result;
}

//-------Static functions----------

/**
 * @brief Init ADC GPIO's
 * @ingroup adc_int
 * @return 0
 */
static int adc_int_gpio_init (void){
    int result = 0;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;

#if(ADC_INT_PWR_EN == 1)
    GPIO_InitStruct.Pin = PWR_CTRL_PIN;
    HAL_GPIO_Init(PWR_CTRL_PORT, &GPIO_InitStruct);
#endif // ADC_INT_PWR_EN

#if(ADC_INT_BAT_EN == 1)
    GPIO_InitStruct.Pin = VBAT_PIN;
    HAL_GPIO_Init(VBAT_PORT, &GPIO_InitStruct);
#endif // ADC_INT_BAT_EN

#if(ADC_INT_VREF_EXT_EN == 1)
    GPIO_InitStruct.Pin = VREF_PIN;
    HAL_GPIO_Init(VREF_PORT, &GPIO_InitStruct);
#endif // ADC_INT_VREF_EXT_EN

    return result;
}

/**
 * @brief Deinit ADC GPIO's
 * @ingroup adc_int
 * @return 0
 */
static int adc_int_gpio_deinit (void){
    int result = 0;

#if(ADC_INT_PWR_EN == 1)
    HAL_GPIO_DeInit(PWR_CTRL_PORT, PWR_CTRL_PIN);
#endif // ADC_INT_PWR_EN

#if(ADC_INT_BAT_EN == 1)
    HAL_GPIO_DeInit(VBAT_PORT, VBAT_PIN);
#endif // ADC_INT_BAT_EN

#if(ADC_INT_VREF_EXT_EN == 1)
    HAL_GPIO_DeInit(VREF_PORT, VREF_PIN);
#endif // ADC_INT_VREF_EXT_EN

    return result;
}

/**
 * @brief Init ADC
 * @param adc_int_pcb - pointer to ADC process control block
 * @ingroup adc_int
 * @return  0 - ok,\n
 *          -1 - HAL_ADC_Init() error,\n
 *          -2 - HAL_ADCEx_Calibration_Start() error,\n
 */
static int adc_int_adc_init(adc_int_pcb_t* adc_int_pcb){
    int result = 0;
    HAL_StatusTypeDef stat = HAL_OK;

    _ADC_INT_CLK_EN();

    //Common config
    adc_int_pcb->hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
    adc_int_pcb->hadc.Init.ContinuousConvMode = ENABLE;
    adc_int_pcb->hadc.Init.DiscontinuousConvMode = DISABLE;
    adc_int_pcb->hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_int_pcb->hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_int_pcb->hadc.Init.NbrOfConversion = 1;

    stat = HAL_ADC_Init(&adc_int_pcb->hadc);
    if (stat != HAL_OK){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_ADC_Init() %S", hal_status[stat]);
    }

    // Config first channel
    static ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = adc_int_pcb->adc_inp[adc_int_pcb->cur_channel];
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

    //Self-calibration
    stat = HAL_ADCEx_Calibration_Start(&adc_int_pcb->hadc);
    if(stat != HAL_OK){
        result = -2;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_ADCEx_Calibration_Start() %S", hal_status[stat]);
    }


    return result;
}

/**
 * @brief Deinit ADC
 * @param adc_int_pcb - pointer to ADC process control block
 * @ingroup adc_int
 * @return  0
 */
static int adc_int_adc_deinit(adc_int_pcb_t* adc_int_pcb){
    int result = 0;

    HAL_ADC_DeInit(&adc_int_pcb->hadc);
    _ADC_INT_CLK_DIS();

    return result;
}

static int adc_int_handle_results(adc_int_pcb_t* adc_int_pcb){
    int result = 0;
    float value = 0.0f;

    if(adc_int_vref_code_avg == NULL){
        result = -1;
    }else{

#if(ADC_INT_PWR_EN == 1)
        value = ADC_INT_VREF_VALUE * adc_int_get_result_avg(adc_int_pcb, ADC_INT_CH_PWR) / *adc_int_vref_code_avg *
                ADC_INT_PWR_A + ADC_INT_PWR_B;
        device.vars.v_pwr = value;
#endif // ADC_INT_PWR_EN

#if(ADC_INT_BAT_EN == 1)
        value = ADC_INT_VREF_VALUE * adc_int_get_result_avg(adc_int_pcb, ADC_INT_CH_BAT) / *adc_int_vref_code_avg *
                ADC_INT_BAT_A + ADC_INT_BAT_B;
        device.vars.v_bat = value;
#endif // ADC_INT_BAT_EN

#if(ADC_INT_TEMP_EN == 1)
        value = ADC_INT_VREF_VALUE * adc_int_get_result_avg(adc_int_pcb, ADC_INT_CH_TEMP) / *adc_int_vref_code_avg *
                ADC_INT_TEMP_A + ADC_INT_TEMP_B;
        device.vars.temperature = value;
#endif // ADC_INT_TEMP_EN

#if(ADC_INT_VREF_INT_EN == 1)
        value = ADC_INT_VREF_VALUE * adc_int_get_result_avg(adc_int_pcb, ADC_INT_CH_VREF_INT) / *adc_int_vref_code_avg;
        device.vars.vref_int = value;
#endif // ADC_INT_VREF_INT_EN

#if(ADC_INT_VREF_EXT_EN == 1)
        value = ADC_INT_VREF_VALUE * adc_int_get_result_avg(adc_int_pcb, ADC_INT_CH_VREF_EXT) / *adc_int_vref_code_avg;
        device.vars.vref_ext = value;
#endif // ADC_INT_VREF_EXT_EN
    }
    return result;
}

/**
 * @brief Get ADC average value of channel
 * @param adc_int_pcb - pointer to ADC process control block
 * @param channel - channel type for search
 * @ingroup adc_int
 * @return ADC average value of given channel
 */
static float adc_int_get_result_avg(adc_int_pcb_t* adc_int_pcb, adc_int_channel_t channel){
    float result = 0.0f;

    for(u8 i = 0; i < ADC_INT_CH_NUM; i++){
        if(adc_int_pcb->adc_channel[i] == channel){
            result = adc_int_pcb->sample[i].value_avg;
        }
    }

    return result;
}

/**
 * @brief Get ADC sample rate of channel
 * @param adc_int_pcb - pointer to ADC process control block
 * @param channel - channel type for search
 * @ingroup adc_int
 * @return ADC sample rate of given channel
 */
static float adc_int_get_sample_rate(adc_int_pcb_t* adc_int_pcb, adc_int_channel_t channel){
    float result = 0.0f;

    for(u8 i = 0; i < ADC_INT_CH_NUM; i++){
        if(adc_int_pcb->adc_channel[i] == channel){
            result = adc_int_pcb->sample[i].sample_rate;
        }
    }

    return result;
}
