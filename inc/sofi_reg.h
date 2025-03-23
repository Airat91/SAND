/* 
 * File:        sofi_reg.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global structs of SOFI
 * Revision history: 0.1
 */

#ifndef SOFI_REG_H
#define SOFI_REG_H 1
 
/*add includes below */
#include "type_def.h"
/*add includes before */
#ifdef __cplusplus 
   extern "C" {
#endif

/**
 * @defgroup sofi_reg
 * @brief Global structs of SOFI
 */

//--------Defines--------

//--------Macro--------

//--------Typedefs-------

//#generator_message{"msg":"sofi_properties", "action":"insert_start"}
// This part of file generated automatically, don't change it

typedef enum{
	SOFI_PROP_BASE,
	SOFI_PROP_MDB,
	SOFI_PROP_RANGE,
	SOFI_PROP_SAVE,
	SOFI_PROP_ACCESS,
}sofi_prop_enum_t;

typedef enum{
	VAR_TYPE_UNKNOWN,                    // Error value        (0 byte)
	VAR_TYPE_U8,                         // uint8_t            (1 byte)
	VAR_TYPE_U16,                        // uint16_t           (2 byte)
	VAR_TYPE_U32,                        // uint32_t           (4 byte)
	VAR_TYPE_U64,                        // uint64_t           (8 byte)
	VAR_TYPE_S8,                         // int8_t             (1 byte)
	VAR_TYPE_S16,                        // int16_t            (2 byte)
	VAR_TYPE_S32,                        // int32_t            (4 byte)
	VAR_TYPE_S64,                        // int64_t            (8 byte)
	VAR_TYPE_FLOAT,                      // float              (4 byte)
	VAR_TYPE_DOUBLE,                     // double             (8 byte)
	VAR_TYPE_STR,                        // char               (1 byte)
}sofi_var_t;

typedef struct{
	sofi_prop_enum_t prop;               // Type of property
	void *           header_next;        // Pointer to next header
	void *           header_base;        // Pointer to base header
}sofi_header_t;

typedef struct{
	sofi_header_t    header;             // Header of property
	u16              prop_num;           // Number of available properties for variable
	u8*              p_value;            // Pointer to value
	char*            name;               // Unique register name
	char*            description;        // Description of variable
	sofi_var_t       type;               // Variable type (see @sofi_var_t enumeration)
	u16              array_len;          // Variable array length (=1 if variable is a single value)
	u16              ind;                // Unique variable index
	u8               read_only;          // If setted, variable can't be changed by external interfaces
}sofi_prop_base_t;

typedef struct{
	sofi_header_t    header;             // Header of property
	u16              mdb_addr;           // ModBUS address of register
}sofi_prop_mdb_t;

typedef struct{
	sofi_header_t    header;             // Header of property
	const void *     p_def;              // Default value
	const void *     p_min;              // Minimum value
	const void *     p_max;              // Maximum value
}sofi_prop_range_t;

typedef struct{
	sofi_header_t    header;             // Header of property
	u16              save_addr;          // Address like byte number in contiguous save array
}sofi_prop_save_t;

typedef struct{
	sofi_header_t    header;             // Header of property
	u8               access_lvl;         // Access level (higher value is strongly protect)
	u32              access_en_timer_ms; // Timer for access (if =0 the access disable)
}sofi_prop_access_t;

//#generator_message{"msg":"sofi_properties", "action":"insert_end"}

//-------External variables------

//-------Function prototypes----------

#ifdef __cplusplus
}
#endif
#endif // SOFI_REG_H
