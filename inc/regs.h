/*
 * File:        regs.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global registers
 * Revision history: 0.1
 */

#ifndef __REGS_H__
#define __REGS_H__
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

//--------Defines--------

//--------Macro--------

//--------Typedefs-------

//-------External variables------

//-------Function prototypes----------

#ifdef __cplusplus
}
#endif
#endif // __REGS_H__
