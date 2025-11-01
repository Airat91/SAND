/*
 * File:        reg.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global registers access
 * Revision history: 0.1
 */

#ifndef __REG_H__
#define __REG_H__
/*add includes below */
#include "sofi_reg.h"
#include "string.h"
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

//=======Regs search functions=======

/**
 * @brief Find register by name
 * @param reg_name - pointer to name for find
 * @ingroup reg
 * @return  - pointer to reg,\n
 *          NULL - register not found,\n
 */
sofi_prop_base_t* reg_get_by_name(char* reg_name);

/**
 * @brief Find register by index
 * @param ind - index for find
 * @ingroup reg
 * @return  - pointer to reg,\n
 *          NULL - register not found,\n
 */
sofi_prop_base_t* reg_get_by_ind(u16 ind);

#ifdef __cplusplus
}
#endif
#endif // __REG_H__
