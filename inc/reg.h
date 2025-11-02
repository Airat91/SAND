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
    sofi_var_t var_type;
}reg_var_t;

//-------External variables------

//-------Function prototypes----------

//=======Regs prop_base functions=======

/**
 * @brief Find register by name
 * @param reg_name - pointer to name for find
 * @ingroup reg
 * @return  - pointer to reg,\n
 *          NULL - register not found,\n
 */
sofi_prop_base_t* reg_base_get_by_name(char* reg_name);

/**
 * @brief Find register by index
 * @param ind - index for find
 * @ingroup reg
 * @return  - pointer to reg,\n
 *          NULL - register not found,\n
 */
sofi_prop_base_t* reg_base_get_by_ind(u16 ind);

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
int reg_base_write(sofi_prop_base_t* reg, u16 array_ind, reg_var_t* value);

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
 *
 * @note
 * This function set only necessary parameters of value struct.
 * Please reset this struct before use.
 *
 * @note
 * Value type after reading sets to the same register type
 */
int reg_base_read(sofi_prop_base_t* reg, u16 array_ind, reg_var_t* value);

//=======Regs prop_mdb functions=======

/**
 * @brief Read register value by address
 * @param addr - address for read
 * @ingroup reg
 * @return  readed value,\n
 *          0 - if access disabled,\n
 */
u16 reg_mdb_read(u16 addr);

/**
 * @brief Read array of register values by start address
 * @param addr - start address for read
 * @param len - lenght for read in u16
 * @param buf - pointer to buffer for reading
 * @ingroup reg
 * @return  0 - ok,\n
 *          negative value if error,\n
 */
int reg_mdb_read_array(u16 addr, u16 len, u16* buf);

/**
 * @brief Read register value by address
 * @param addr - address for read
 * @param value - value for write
 * @ingroup reg
 * @return  0 - ok,\n
 *          negative value if error,\n
 */
int reg_mdb_write_reg(u16 addr, u16 value);

/**
 * @brief Write to array of register by start address
 * @param addr - start address for write
 * @param len - lenght for write array in u16
 * @param buf - pointer to buffer with write data
 * @ingroup reg
 * @return  0 - ok,\n
 *          negative value if error,\n
 */
int reg_mdb_write_array(u16 addr, u16 len, u16* buf);

//=======Regs prop_range functions=======

//=======Regs prop_save functions=======

//=======Regs prop_access functions=======

/**
 * @brief Check register access
 * @param reg - pointer to reg
 * @ingroup reg
 * @return  0 - access enable,\n
 *          1 - access blocked,\n
 */
int reg_access_blocked(sofi_prop_base_t* reg);

#ifdef __cplusplus
}
#endif
#endif // __REG_H__
