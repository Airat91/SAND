/*
 * File:        reset_sand.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Reset functions
 * Revision history: 0.1
 */

#ifndef __RESET_SAND_H__
#define __RESET_SAND_H__
/*add includes below */
#include "type_def.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_pwr.h"
#include "reg.h"
#include "main.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup reset
 * @brief Reset functions
 */

//--------Defines--------

#define RESET_LOWPOWER_EN_DELAY_MS  1000    // Delay after power up (1 sec)
#define RESET_SHUTDOWN_DELAY_MS     5000    // 5 second

//--------Macro--------

//--------Typedefs-------

typedef enum{
    RST_REASON_PIN      = 0x01, //
    RST_REASON_PWR_ON   = 0x02, //
    RST_REASON_SOFTWARE = 0x04, //
    RST_REASON_IWDG     = 0x08, //
    RST_REASON_WWDG     = 0x10, //
    RST_REASON_PWR_OFF  = 0x20, //
}reset_sand_reason_t;

//-------External variables------

//-------Function prototypes----------

/**
 * @brief Read reason of reset and clear RCC_CSR register
 * @param reason - pointer for write result
 * @ingroup reset
 * @return 0
 */
int reset_get_reason(reset_sand_reason_t* reason);

/**
 * @brief Software reset
 * @ingroup reset
 * @return 0
 *
 * @details
 * Disable all peripherials
 * Stop all tasks
 * Update total runtime and reset num in vars
 * Save storage
 */
int reset_system(void);

/**
 * @brief Init low power monitor
 * @ingroup reset
 * @return  0 - ok,\n
 *          negative value if error,\n
 */
int reset_lowpower_init(void);

/**
 * @brief Call this function from EXTI Low Power interrupt
 * @ingroup reset
 * @return 0
 */
int reset_lowpower_irq_callback(void);

/**
 * @brief Call this function over 1 second after power up
 * @ingroup reset
 * @return 0
 */
int reset_lowpower_save_enable(void);

#ifdef __cplusplus
}
#endif
#endif // __RESET_SAND_H__
