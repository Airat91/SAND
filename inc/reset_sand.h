/*
 * File:        reset_sand.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Reset status reading
 * Revision history: 0.1
 */

#ifndef __RESET_SAND_H__
#define __RESET_SAND_H__
/*add includes below */
#include "type_def.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "reg.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup reset
 * @brief Reset status reading
 */

//--------Defines--------

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

#ifdef __cplusplus
}
#endif
#endif // __RESET_SAND_H__
