/*
 * File:        reg.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global registers access
 * Revision history: 0.1
 */

#ifndef __REG_H__
#define __REG_H__
/*add includes below */
#include "sand_reg.h"
#include "string.h"
#include "cmsis_os.h"
#include "storage.h"
#if(DEVICE_TYPE == DEFPCB)
    #include "rergs_defpcb.h"
#elif(DEVICE_TYPE == SAND_AI)
    #include "regs_sand_ai.h"
#elif(DEVICE_TYPE == BRIOM_2AI)
    #include "regs_briom_2ai.h"
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

typedef union{
    u8      var_u8;
    u16     var_u16;
    u32     var_u32;
    u64     var_u64;
    s8      var_s8;
    s16     var_s16;
    s32     var_s32;
    s64     var_s64;
    float   var_float;
    double  var_double;
    char    var_char;
}reg_var_union_t;

typedef struct{
    reg_var_union_t  var;
    sand_var_t var_type;
}reg_var_t;

//-------External variables------

extern osMutexId regs_access_mutex;

//-------Function prototypes----------

//=======Regs prop_base functions=======

/**
 * @brief Find register by name
 * @param reg_name - pointer to name for find
 * @ingroup reg
 * @return  - pointer to reg,\n
 *          NULL - register not found,\n
 */
sand_prop_base_t* reg_base_get_by_name(char* reg_name);

/**
 * @brief Find register by index
 * @param ind - index for find
 * @ingroup reg
 * @return  - pointer to reg,\n
 *          NULL - register not found,\n
 */
sand_prop_base_t* reg_base_get_by_ind(u16 ind);

/**
 * @brief Write new value to register (register array)
 * @param reg - pointer to register
 * @param array_ind - register array index (set 1 if register isn't array)
 * @param value - pointer to value
 * @ingroup reg
 * @return  0 - ok,\n
 *          -1 - reg is NULL,\n
 *          -2 - access blocked,\n
 *          -3 - array index out of lenght,\n
 *          -4 - value type mismatch,\n
 *          -5 - register read only,\n
 */
int reg_base_write(sand_prop_base_t* reg, u16 array_ind, reg_var_t* value);

/**
 * @brief Read register value (register array)
 * @param reg - pointer to register
 * @param array_ind - register array index (set 1 if register isn't array)
 * @param value - pointer to value
 * @ingroup reg
 * @return  0 - ok,\n
 *          -1 - reg is NULL,\n
 *          -2 - access blocked,\n
 *          -3 - array index out of lenght,\n
 * @details
 * 1. In error the value is sets to 0, type is sets to VAR_TYPE_UNKNOWN
 * 2. This function set only necessary parameters of value struct.
 * Please reset this struct before use.
 * 3. Value type after reading sets to the same register type
 */
int reg_base_read(sand_prop_base_t* reg, u16 array_ind, reg_var_t* value);

/**
 * @brief reg_get_byte_num
 * @param reg - pointer to register
 * @ingroup reg
 * @return number in bytes of var type
 */
int reg_base_get_byte_size(sand_prop_base_t* reg);
/**
 * @brief Get property header of register
 * @param reg - pointer to register
 * @param prop - property type
 * @ingroup reg
 * @return  - pointer to property header,\n
 *          NULL - property header not found,\n
 */
sand_header_t* reg_base_get_prop(sand_prop_base_t* reg, sand_prop_enum_t prop);

//=======Regs prop_mdb functions=======

/**
 * @brief Find register by ModBUS address
 * @param addr - address for find
 * @ingroup reg
 * @return  - pointer to reg,\n
 *          NULL - register not found,\n
 */
sand_prop_base_t* reg_mdb_get_by_addr(u16 addr);

//=======Regs prop_range functions=======

/**
 * @brief Correct value with register min - max range
 * @param reg - pointer to register
 * @param value - pointer to value
 * @ingroup reg
 * @return 0
 */
int reg_range_min_max_correct(sand_prop_base_t* reg, reg_var_t* value);

//=======Regs prop_save functions=======

/**
 * @brief Check register is saveable
 * @param reg - pointer to register
 * @ingroup reg
 * @return  0 - register is saveable,\n
 *          -1 - register is not saveable,\n
 * @details
 */
int reg_save_check(sand_prop_base_t* reg);

//=======Regs prop_access functions=======

/**
 * @brief Check register access
 * @param reg - pointer to reg
 * @ingroup reg
 * @return  0 - access enable,\n
 *          1 - access blocked,\n
 */
int reg_access_blocked(sand_prop_base_t* reg);

#ifdef __cplusplus
}
#endif
#endif // __REG_H__
