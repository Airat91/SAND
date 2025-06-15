/* 
 * File:        us_tim.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: 32-bit us timer api function
 * Revision history: 0.1
 */

#ifndef US_TIM_H
#define	US_TIM_H

/*add includes below */
#include "type_def.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "debug.h"
/*add includes before */

#ifdef	__cplusplus
extern "C" {
#endif
    
/**
 * @defgroup us_tim
 * @brief 32-bit us timer api functions
 */
    
//--------Defines--------

#define US_TIM_YIELD_THRESHOLD      100
#define US_TIM_FREQ                 1000000     // 1 MHz

//--------Macro--------

#define TIM_US                      TIM2
#define TIM_US_IRQn                 TIM2_IRQn
#define __TM_US_CLK_ENABLE()        __HAL_RCC_TIM2_CLK_ENABLE()

//--------Typedefs-------

//-------External variables------

//-------Function prototypes----------

/**
 * @brief Init us timer
 * @ingroup us_tim
 * @return  0 - ok,\n
 *          -1 - HAL_TIM_Base_Init() error,\n
 *          -2 - HAL_TIM_ConfigClockSource() error,\n
 *          -3 - HAL_TIMEx_MasterConfigSynchronization() error,\n
 */
int us_tim_init(void);

/**
 * @brief Deinit us timer
 * @ingroup us_tim
 */
void us_tim_deinit(void);
/**
 * @brief Get value from global us timer
 * @return global us timer value
 * @ingroup us_tim
 */
u32 us_tim_get_value(void);

/**
 * @brief Us delay in blocking mode
 * @param us - delay value in us
 * @ingroup us_tim
 */
void us_tim_delay(u32 us);

/**
 * @brief Call this function from us_im IRQ
 * @ingroup us_tim
 *
 * Increase global us_cnt_H value
 */
void us_tim_irq_handler(void);

#ifdef __cplusplus
}
#endif

#endif /* US_TIM_H */
