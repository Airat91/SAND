/*
 * File:        ai.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Work with AI channels based on internal ADC
 * Revision history: 0.2
 *      0.2 - Added pseudo buffering algorithm
 */

#ifndef AI_H
#define AI_H 1
/*add includes below */
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"
#include "stm32f1xx_hal_gpio.h"
#include "type_def.h"
#include "pin_map.h"
#include "reg.h"
#include "sand_reg.h"
#include "debug.h"
#include "adc_int.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup ai
 * @brief Work with AI channels based on internal ADC
 */

//--------Defines--------

#define AI_MODULE_VER               {0, 2}  // See revision history in file header
#define AI_MAX_ERR_NMB              50      // Maximum errors numbers in a row for reinit
#define AI_SAMPLE_MAX_NUM           50      // Samples number for averaging
#define AI_INIT_TIMEOUT_MS          2000    // 2 sec
#define AI_ADC_MAX_RANGE            4096    // ADC code max value
#define AI_TASK_PERIOD              10      // Equal measurement period in ms
#define AI_CH_NUM                   2       // Module have 2 Analog Inputs
#define AI_AVG_BUF_EN               0       // Use buffers for samples averaging

//========Vref configuration========
#define AI_VREF_USE                 ADC_INT_VREF_USE    // AI_ADC use the same VREF than ADC_INT

//--------Macro--------

#define _AI_ADC                     ADC2
#define _AI_ADC_CLK_EN              __HAL_RCC_ADC2_CLK_ENABLE
#define _AI_ADC_CLK_DIS             __HAL_RCC_ADC2_CLK_DISABLE
#define _AI_ADC_IRQn                ADC1_2_IRQn
#define _AI_CHANNEL_0               ADC_CHANNEL_0
#define _AI_CHANNEL_1               ADC_CHANNEL_5
#define _AI_ADC_SAMPLETIME          ADC_SAMPLETIME_239CYCLES_5

//========Check GPIO's definitions========
#ifndef AI_MCU_0_PORT
    #error "AI: Please define AI_MCU_0_PORT"
#endif
#ifndef AI_MCU_0_PIN
    #error "AI: Please define AI_MCU_0_PIN"
#endif
#ifndef AI_MCU_1_PORT
    #error "AI: Please define AI_MCU_1_PORT"
#endif
#ifndef AI_MCU_1_PIN
    #error "AI: Please define AI_MCU_1_PIN"
#endif

//--------Typedefs-------

typedef struct{
#if(AI_AVG_BUF_EN == 1)
    u16 value[AI_SAMPLE_MAX_NUM];   // ADC code values array
    u32 period[AI_SAMPLE_MAX_NUM];  // Time periods array between two samples in us
    u16 sample_ptr;                 // Pointer of buffers
#endif // AI_AVG_BUF_EN
    u32 last_time;                  // Last time of sample in us
    u16 sample_avg_divider;         // For correct averaging
    float value_avg;                // Average value of ADC measurement
    float sample_rate;              // Samples per second (Hz)
}ai_sample_t;

typedef enum{
    AI_ST_DISABLE  = 0,             // ADC disable
    AI_ST_READY    = (1<<0),        // ADC ready
    AI_ST_ERROR    = (1<<1),        // Error state, reinit need
}ai_state_t;

typedef struct{
    ai_state_t state;               // State of ADC pcb
    ADC_HandleTypeDef hadc;         // Struct fol HAL driver
    u16 err_cnt;                    // Counter of errors
    u16 cur_channel;                // Selected channel number
    u32 adc_inp[AI_CH_NUM];         // List of ADC channels for HAL-driver
    ai_sample_t sample[AI_CH_NUM];  // list of ADC channels structs
    u8 cycle_done;                  // Flag sets after full cycle ended
}ai_pcb_t;

//-------External variables------

extern osThreadId ai_task_handle;
extern ai_pcb_t ai_pcb;        // Internal ADC process control block

//-------Function prototypes----------

/**
 * @brief Main task of internal ADC
 * @param argument - a pointer to arguments
 * @ingroup ai
 */
void ai_task(void const * argument);

/**
 * @brief Init AI ADC and process control block
 * @param ai_pcb - pointer to process control block
 * @ingroup ai
 * @return  0 - ok,\n
 *          -1 - ai_gpio_init() error,\n
 *          -2 - ai_adc_init() error,\n
 *
 * @details
 * 1. Init GPIO's
 * 2. Init ADC
 * 3. Enable interrupts
 */
int ai_init(ai_pcb_t* ai_pcb);

/**
 * @brief Denit AI ADC and process control block
 * @param ai_pcb - pointer to process control block
 * @ingroup ai
 * @return  0 - ok,\n
 *          negative value - error
 *
 * @details
 * 1. Disable interrupts
 * 2. Stop ADC
 * 3. Deinit GPIO's
 * 4. Deinit ADC
 */
int ai_deinit(ai_pcb_t* ai_pcb);

/**
 * @brief Call this function from ADC interrupt
 * @param ai_pcb - pointer to process control block
 * @ingroup ai
 * @return  0 - ok,\n
 *          negative value - error
 */
int ai_irq_callback(ai_pcb_t* ai_pcb);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // AI_H
