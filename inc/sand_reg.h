/* 
 * File:        sand_reg.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global structs of SOFI
 * Revision history: 0.1
 */

#ifndef SAND_REG_H
#define SAND_REG_H 1
 
/*add includes below */
#include "type_def.h"
/*add includes before */
#ifdef __cplusplus 
   extern "C" {
#endif

/**
 * @defgroup sand_reg
 * @brief Global structs of SAND
 */

//--------Defines--------

//--------Macro--------

//--------Typedefs-------

//#generator_message{"msg":"sand_properties", "action":"insert_start"}
// This part of file generated automatically, don't change it

typedef enum{
	SAND_PROP_BASE,
	SAND_PROP_MDB,
	SAND_PROP_RANGE,
	SAND_PROP_SAVE,
	SAND_PROP_ACCESS,
	SAND_PROP_CALLBACK,
}sand_prop_enum_t;

typedef enum{
	VAR_TYPE_UNKNOWN,                      // Error value        (0 byte)
	VAR_TYPE_U8,                           // uint8_t            (1 byte)
	VAR_TYPE_U16,                          // uint16_t           (2 byte)
	VAR_TYPE_U32,                          // uint32_t           (4 byte)
	VAR_TYPE_U64,                          // uint64_t           (8 byte)
	VAR_TYPE_S8,                           // int8_t             (1 byte)
	VAR_TYPE_S16,                          // int16_t            (2 byte)
	VAR_TYPE_S32,                          // int32_t            (4 byte)
	VAR_TYPE_S64,                          // int64_t            (8 byte)
	VAR_TYPE_FLOAT,                        // float              (4 byte)
	VAR_TYPE_DOUBLE,                       // double             (8 byte)
	VAR_TYPE_CHAR,                         // char               (1 byte)
}sand_var_t;

typedef enum{
	ACC_LVL_ROOT       = 0,                // Only for developers
	ACC_LVL_ADMIN_HIGH = 1,                // Administrator  with high priority
	ACC_LVL_ADMIN_LOW  = 2,                // Administrator
	ACC_LVL_USER_HIGH  = 3,                // User with high priority
	ACC_LVL_USER_LOW   = 4,                // User
}sand_access_lvl;

typedef struct{
	sand_prop_enum_t   prop;               // Type of property
	void *             header_next;        // Pointer to next header
	void *             header_base;        // Pointer to base header
}sand_header_t;

typedef struct{
	sand_header_t      header;             // Header of property
	u16                prop_num;           // Number of available properties for variable
	u8*                p_value;            // Pointer to value
	char*              name;               // Unique register name
	char*              description;        // Description of variable
	sand_var_t         type;               // Variable type (see @sand_var_t enumeration)
	u16                array_len;          // Variable array length (=1 if variable is a single value)
	u16                ind;                // Unique variable index
	u8                 read_only;          // If setted, variable can't be changed by external interfaces
}sand_prop_base_t;

typedef struct{
	sand_header_t      header;             // Header of property
	u16                mdb_addr;           // ModBUS address of register
}sand_prop_mdb_t;

typedef struct{
	sand_header_t      header;             // Header of property
	const void *       p_def;              // Default value
	const void *       p_min;              // Minimum value
	const void *       p_max;              // Maximum value
}sand_prop_range_t;

typedef struct{
	sand_header_t      header;             // Header of property
	u16                save_addr;          // Address like byte number in continuous save array
	u8                 busy;               // Register is busy in saving operation flag
}sand_prop_save_t;

typedef struct{
	sand_header_t      header;             // Header of property
	u8                 access_lvl;         // Access level (lower value is strongly protect)
	u32                access_en_timer_ms; // Timer for access (if =0 the access disable)
}sand_prop_access_t;

typedef struct{
	sand_header_t      header;             // Header of property
	void               (*callback)(void);  // Pointer to callback-function
	u8                 only_end;           // Call callback only if last array-element changed (if =0 any element changing will call callback)
}sand_prop_callback_t;


//#generator_message{"msg":"sand_properties", "action":"insert_end"}

//-------External variables------

//-------Function prototypes----------

#ifdef __cplusplus
}
#endif
#endif // SAND_REG_H
