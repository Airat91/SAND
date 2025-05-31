/*
 * File:        hal_status.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: HAL_StatusTypeDef values description
 * Revision history: 0.1
 */

#ifndef HAL_STATUS_H
#define	HAL_STATUS_H

/*add includes below */
#include "stm32f1xx_hal_def.h"
/*add includes before */

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @defgroup hal_status
 * @brief HAL_StatusTypeDef values description
 */

//--------Defines--------

#define HAL_STATUS_NUM  4
#define HAL_STATUS_LEN  10

//--------Macro--------

//--------Typedefs-------

//-------External variables------

extern const char hal_status[HAL_STATUS_NUM][HAL_STATUS_LEN];

//-------Function prototypes----------

#ifdef	__cplusplus
}
#endif

#endif // HAL_STATUS_H
