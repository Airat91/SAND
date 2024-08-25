#ifndef REGS_H
#define REGS_H

#include "sand_config.h"
#include "type_def.h"
#include "stddef.h"

#if(MODULE == AO_2)
    #include "regs_ao_2.h"
#elif(MODULE == AI_2)
    #include "regs_ai_2.h"
#elif(MODULE == DI_2)
    #include "regs_di_2.h"
#elif(MODULE == DO_2)
    #include "regs_do_2.h"
#elif(MODULE == DOR_1)
    #include "regs_dor_1.h"
#endif // MODULE

//========== Regs options enumerations ==========

/**
 * @brief Available extended options for regs
 * @ingroup regs_opt
 */
typedef enum{
    OPT_VAL = 0,    // value, default, properties, etc.
    OPT_SAVE,       // save address and lenght
    OPT_MENU,       // params for change value by menu
    OPT_MDB,        // ModBUS parameters
    OPT_CAN,        // CAN parameters
    OPT_LIMIT,      // max and min values
    OPT_ACCESS,     // change protection, reset protection, etc.

//--------- add new options before ---------
    OPT_NUM,        // Number of options
}reg_opt_t;

/**
 * @brief Available types for regs
 * @ingroup regs_opt
 */
typedef enum{
    TYPE_U8 = 10,
    TYPE_S8,
    TYPE_U16,
    TYPE_S16,
    TYPE_U32,
    TYPE_S32,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
}reg_type_t;

//========== Regs options structs ==========

/**
 * @brief Commin header for opts struct
 * @ingroup regs_opt
 */
typedef struct MCU_PACK{
    const void* p_next_opt;     // pointer to next option struct or NULL if no more options
    const reg_opt_t opt_type;   // option descriptor
    const void* p_parent;       // pointer to parent reg (val_opt_t)
}opt_head_t;

/**
 * @brief Main mandatory regs parameters
 * @ingroup regs_opt
 */
typedef struct MCU_PACK{
    const opt_head_t opt;       // option header

    void* p_value;              // pointer to variable
    char* name;                 // unique register name
    char* description;          // pointer to description string
    u16 type;                   // variable type (see reg_type_t enumeration)
    u16 ind;                    // unique register index
    const void* p_default;      // pointer to default value

}opt_val_t;

/**
 * @brief Parameters for save in storage
 * @ingroup regs_opt
 */
typedef struct MCU_PACK{
    const opt_head_t opt;       // option header

    const u32 addr;             // address in data storage
    const u16 len;              // total size in bytes
}opt_save_t;

/**
 * @brief Parameters for change reg value from HMI menu
 * @ingroup regs_opt
 */
typedef struct MCU_PACK{
    const opt_head_t opt;       // option header

}opt_menu_t;

/**
 * @brief Parameters for ModBUS communication
 * @ingroup regs_opt
 */
typedef struct MCU_PACK{
    const opt_head_t opt;       // option header

}opt_mdb_t;

/**
 * @brief Parameters for change reg value from HMI menu
 * @ingroup regs_opt
 */
typedef struct MCU_PACK{
    const opt_head_t opt;       // option header

}opt_can_t;

/**
 * @brief Max and min value limits
 * @ingroup regs_opt
 */
typedef struct MCU_PACK{
    const opt_head_t opt;       // option header

    const void* max;
    const void* min;
}opt_limit_t;

/**
 * @brief Parameters for reg access
 * @ingroup regs_opt
 */
typedef struct MCU_PACK{
    const opt_head_t opt;       // option header

    const u8 level;             // access level
    u8 locked;                  // {0-possible to change, 1-change is locked}
}opt_access_t;

//========== External variables ==========

extern opt_val_t const regs_list[];

//========== Function declarations ==========


#endif // REGS_H
