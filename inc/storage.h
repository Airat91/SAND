/*
 * File:        storage.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Work with sand storage
 * Revision history: 0.1
 */

#ifndef STORAGE_H
#define STORAGE_H 1
/*add includes below */
#include "cmsis_os.h"
#include "type_def.h"
#include "reg.h"
#include "sofi_reg.h"
#include "debug.h"
#include "flash.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup storage
 * @brief Work with sand storage
 */

//--------Defines--------

#define STORAGE_FLASH_SIZE          0x0001000   // 4K bytes
#define STORAGE_FLASH_START         FLASH_START + FLASH_TOTAL_SIZE - FLASH_SAVE_AREA_SIZE
#define STORAGE_FLASH_END           STORAGE_FLASH_START + STORAGE_FLASH_SIZE
#define STORAGE_FLASH_PAGE_NMB      FLASH_SAVE_AREA_SIZE / FLASH_PAGE_SIZE

//--------Macro--------

//--------Typedefs-------

typedef struct{

}storage_header_t;

typedef struct{

}storage_pcb_t;

//-------External variables------

//-------Function prototypes----------

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // STORAGE_H
