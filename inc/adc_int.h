/*
 * File:        adc_int.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Work with internal ADC
 * Revision history: 0.1
 */

#ifndef ADC_INT_H
#define ADC_INT_H 1
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
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup adc_int
 * @brief Work with internal ADC
 */

//--------Defines--------

#define ADC_INT_MAX_ERR_NMB         50      // Maximum errors numbers in a row for reinit
#define ADC_INT_SAMPLE_NUM          10      // Samples number for averaging
#define ADC_INT_MAX_RANGE           4096    // ADC code max value
#define ADC_INT_TASK_PERIOD         100     // Equal measurement period in ms
#define ADC_INT_INIT_TIMEOUT_MS     2000    // 2 sec

//========Vref configuration========
#define ADC_INT_VREF_INT            0x01    // Vref code for select
#define ADC_INT_VREF_EXT            0x02    // Vref code for select
#define ADC_INT_VREF_USE            ADC_INT_VREF_EXT    // Select ADC_VREF_INT or ADC_VREF_EXT
#define ADC_INT_VREF_INT_VALUE      1.2f    // Internal voltage reference value
#define ADC_INT_VREF_EXT_VALUE      2.5f    // External voltage reference value
#if(ADC_INT_VREF_USE == ADC_INT_VREF_INT)
    #define ADC_INT_VREF_VALUE      ADC_INT_VREF_INT_VALUE
#elif(ADC_INT_VREF_USE == ADC_INT_VREF_EXT)
    #define ADC_INT_VREF_VALUE      ADC_INT_VREF_EXT_VALUE
#else
    #error "ADC: Please define PWR_CTRL_PORT"
#endif // ADC_INT_VREF_USE

//========Internal channels selection========
#define ADC_INT_PWR_EN              1       // Enable Power voltage measurement
#define ADC_INT_BAT_EN              0       // Enable Battery voltage measurement
#define ADC_INT_TEMP_EN             1       // Enable MCU temperature measurement
#define ADC_INT_VREF_INT_EN         1       // Enable internal Voltage reference
#define ADC_INT_VREF_EXT_EN         1       // Enable external Voltage reference
#define ADC_INT_CH_NUM              ADC_INT_PWR_EN+ADC_INT_BAT_EN+ADC_INT_TEMP_EN+ADC_INT_VREF_INT_EN+ADC_INT_VREF_EXT_EN

//========Internal channels parameters========
#define ADC_INT_PWR_A               16.0f   // Koef of function Vlt = (VREF_VALUE * ADC / VREF_CODE) * A + B
#define ADC_INT_PWR_B               0.0f    // Koef of function Vlt = (VREF_VALUE * ADC / VREF_CODE) * A + B

#define ADC_INT_BAT_A               1.0f    // Koef of function Vlt = (VREF_VALUE * ADC / VREF_CODE) * A + B
#define ADC_INT_BAT_B               0.0f    // Koef of function Vlt = (VREF_VALUE * ADC / VREF_CODE) * A + B
#define ADC_INT_BAT_PERIOD_US       60000   // Battery measurement period

#define ADC_INT_TEMP_A              -232.55814f // Koef of function Tmpr = (VREF_VALUE * ADC / VREF_CODE) * A + B
#define ADC_INT_TEMP_B              375.55814f  // Koef of function Tmpr = (VREF_VALUE * ADC / VREF_CODE) * A + B
                                            // Temperature (in °C) = {(V_25 - V SENSE ) / Avg_Slope} + 25.
                                            // Avg_Slope = 4.3 (4.0 - 4.6) mV/°C
                                            // V_25 = 1.43 (1.34 - 1.52) V
                                            // The V_25 varies from chip to chip due to process variation (up to 45 °C
                                            // from one chip to another) so use individual koef B from regs

//--------Macro--------

#define _ADC_INT                    ADC1
#define _ADC_INT_CLK_EN             __HAL_RCC_ADC1_CLK_ENABLE
#define _ADC_INT_CLK_DIS            __HAL_RCC_ADC1_CLK_DISABLE
#define _ADC_INT_IRQn               ADC1_2_IRQn
#define _ADC_CHANNEL_PWR            ADC_CHANNEL_1
#define _ADC_CHANNEL_BAT            NULL
#define _ADC_CHANNEL_VREF_EXT       ADC_CHANNEL_6
#define _ADC_CHANNEL_VREF_INT       ADC_CHANNEL_VREFINT
#define _ADC_CHANNEL_TEMP           ADC_CHANNEL_TEMPSENSOR

//========Check GPIO's definitions========
#if(ADC_INT_PWR_EN == 1)
#ifndef PWR_CTRL_PORT
    #error "ADC: Please define PWR_CTRL_PORT"
#endif
#ifndef PWR_CTRL_PIN
    #error "ADC: Please define PWR_CTRL_PIN"
#endif
#endif // ADC_INT_PWR_EN

#if(ADC_INT_BAT_EN)
#ifndef VBAT_PORT
    #error "ADC: Please define VBAT_PORT"
#endif
#ifndef VBAT_PIN
    #error "ADC: Please define VBAT_PIN"
#endif
#endif // ADC_INT_BAT_EN

#if(ADC_INT_VREF_EXT_EN)
#ifndef VREF_PORT
    #error "ADC: Please define VREF_PORT"
#endif
#ifndef VREF_PIN
    #error "ADC: Please define VREF_PIN"
#endif
#endif // ADC_INT_VREF_EXT_EN

//--------Typedefs-------

typedef struct{
   u16 value[ADC_INT_SAMPLE_NUM];   // ADC code values array
   u32 period[ADC_INT_SAMPLE_NUM];  // Time periods array between two samples in us
   u32 last_time;                   // Last time of sample in us
   u16 sample_ptr;                  // Pointer of buffers
   u16 sample_avg_divider;          // For correct averaging
   float value_avg;                 // Average value of ADC measurement
   float sample_rate;               // Samples per second (Hz)
}adc_int_sample_t;

typedef enum {
    ADC_INT_CH_PWR = 0,             // Power voltage
    ADC_INT_CH_BAT,                 // Battery voltage
    ADC_INT_CH_TEMP,                // MCU temperature
    ADC_INT_CH_VREF_INT,            // Internal voltage reference
    ADC_INT_CH_VREF_EXT,            // External volrage reference
}adc_int_channel_t;

typedef enum{
    ADC_INT_ST_DISABLE  = 0,        // ADC disable
    ADC_INT_ST_READY    = (1<<0),   // ADC ready
    ADC_INT_ST_ERROR    = (1<<1),   // Error state, reinit need
}adc_int_state_t;

typedef struct{
    adc_int_state_t state;          // State of ADC pcb
    ADC_HandleTypeDef hadc;         // Struct fol HAL driver
    u16 err_cnt;                    // Counter of errors
    u16 cur_channel;                // Selected channel number
    u32 adc_inp[ADC_INT_CH_NUM];    // List of ADC channels for HAL-driver
    adc_int_channel_t adc_channel[ADC_INT_CH_NUM];  // List of internal ADC channels
    adc_int_sample_t sample[ADC_INT_CH_NUM];        // list of ADC channels structs
    u8 cycle_done;                  // Flag sets after full cycle ended
}adc_int_pcb_t;

//-------External variables------

extern osThreadId adc_int_task_handle;
extern adc_int_pcb_t adc_int_pcb;   // Internal ADC process control block
extern float* adc_int_vref_code_avg;// Pointer to voltage reference averaged code

//-------Function prototypes----------

/**
 * @brief Main task of internal ADC
 * @param argument - a pointer to arguments
 * @ingroup adc_int
 */
void adc_int_task(void const * argument);

/**
 * @brief Init internal ADC and process control block
 * @param adc_int_pcb - pointer to process control block
 * @ingroup adc_int
 * @return  0 - ok,\n
 *          -1 - adc_int_gpio_init() error,\n
 *          -2 - adc_int_adc_init() error,\n
 *
 * @details
 * 1. Init GPIO's
 * 2. Init ADC
 * 3. Enable interrupts
 */
int adc_int_init (adc_int_pcb_t* adc_int_pcb);

/**
 * @brief Denit internal ADC and process control block
 * @param adc_int_pcb - pointer to process control block
 * @ingroup adc_int
 * @return  0 - ok,\n
 *          negative value - error
 *
 * @details
 * 1. Disable interrupts
 * 2. Stop ADC
 * 3. Deinit GPIO's
 * 4. Deinit ADC
 */
int adc_int_deinit (adc_int_pcb_t* adc_int_pcb);

/**
 * @brief Call this function from ADC interrupt
 * @param adc_int_pcb - pointer to process control block
 * @ingroup adc_int
 * @return  0 - ok,\n
 *          negative value - error
 */
int adc_int_irq_callback(adc_int_pcb_t* adc_int_pcb);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ADC_INT_H
