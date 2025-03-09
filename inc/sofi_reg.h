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

typedef enum{
	VAR_TYPE_UNKNOWN = 0,		// Error value
    VAR_TYPE_U8,                // uint8_t		(1 byte)
    VAR_TYPE_U16,               // uint16_t		(2 byte)
    VAR_TYPE_U32,               // uint32_t		(4 byte)
    VAR_TYPE_U64,               // uint64_t		(8 byte)
    VAR_TYPE_S8,                // int8_t		(1 byte)
    VAR_TYPE_S16,               // int16_t		(2 byte)
    VAR_TYPE_S32,               // int32_t 		(4 byte)
    VAR_TYPE_S64,               // int64_t 		(8 byte)
	VAR_TYPE_FLOAT,				// float 		(4 byte)
    VAR_TYPE_DOUBLE,            // double 		(8 byte)
}sofi_var_type_t;

//#generator_message{"msg":"sofi_properties", "action":"insert_start"}
// This part of file generated automatically, don't change it
//#generator_message{"msg":"sofi_properties", "action":"insert_end"}

//-------External variables------

//-------Function prototypes----------

#ifdef __cplusplus
}
#endif
#endif // SOFI_REG_H
