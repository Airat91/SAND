/*
 * File:        reg.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global registers access
 * Revision history: 0.1
 */

#ifndef __REG_H__
#define __REG_H__
/*add includes below */
#if(DEVICE_TYPE == DEFPCB)
    #include "rergs_defpcb.h"
#elif(DEVICE_TYPE == SAND_AI)
    #include "regs_sand_ai.h"
#endif // DEVICE_TYPE
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup reg
 * @brief Global registers access
 */

//--------Defines--------

//--------Macro--------

//--------Typedefs-------

//-------External variables------

//-------Function prototypes----------

#ifdef __cplusplus
}
#endif
#endif // __REG_H__
