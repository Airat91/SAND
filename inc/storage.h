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
#include "sand_reg.h"
#include "debug.h"
#include "flash.h"
#include "stm32f1xx_hal_crc.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup storage
 * @brief Work with sand storage
 */

//--------Defines--------

#define STORAGE_FLASH_SIZE          0x00001000  // 4K bytes
#define STORAGE_FLASH_START         FLASH_START + FLASH_TOTAL_SIZE - STORAGE_FLASH_SIZE
#define STORAGE_FLASH_END           STORAGE_FLASH_START + STORAGE_FLASH_SIZE
#define STORAGE_FLASH_PAGE_NMB      FLASH_SAVE_AREA_SIZE / FLASH_PAGE_SIZE
#define STORAGE_MUTEX_TIMEOUT       100

//--------Macro--------

//--------Typedefs-------

typedef struct{
    u32* next_header;               // Pointer to next storage header
    u16 data_crc;                   // CRC of storage data without header, used for storage data validation
    u32 names_crc;                  // CRC of all saved registers names, used for restore
    u16 data_len;                   // Storage data lenght in bytes
    u16 erase_cnt;                  // Full erase of storage area counter
}storage_header_t;

typedef struct{
    storage_header_t header;        // Storage header
    u8 data;                        // Storage data first element
}storage_dump_t;

typedef struct{
    u8 data_changed;                // Set this flag after regs change
    u32 last_save_time_ms;          // Time of last data save
    storage_dump_t* dump;           // Pointer to last actual damp in storage area
    u32 current_names_crc;
}storage_pcb_t;

//-------External variables------

extern storage_pcb_t storage_pcb;
extern osMutexId regs_storage_mutex;

//-------Function prototypes----------

/**
 * @brief Init storage control block
 * @param storage_pcb - pointer to storage process control block
 * @param reg_list - pointer to list of regs
 * @param reg_list_len - lenght of reg_list
 * @ingroup storage
 * @return  0 - ok,\n
 *          negative value if error,\n
 */
int storage_init(storage_pcb_t* storage_pcb, const sand_prop_save_t* reg_list, u16 reg_list_len);

/**
 * @brief Restore data from storage
 * @param storage_pcb - pointer to storage process control block
 * @param reg_list - pointer to list of regs
 * @param reg_list_len - lenght of reg_list
 * @ingroup storage
 * @return  0 - ok,\n
 *          negative value if error,\n
 *
 * @details
 * If storage is empty, sets default values or null
 */
int storage_restore_data(storage_pcb_t* storage_pcb, const sand_prop_save_t* reg_list, u16 reg_list_len);

/**
 * @brief Save all values of regs in storage
 * @param storage_pcb - pointer to storage process control block
 * @param reg_list - pointer to list of regs
 * @param reg_list_len - lenght of reg_list
 * @ingroup storage
 * @return  0 - ok,\n
 *          negative value if error,\n
 *
 * @details
 * During saving time sets save_busy flags and block regs for changing via reg_base_write()
 */
int storage_save_data(storage_pcb_t* storage_pcb, const sand_prop_save_t* reg_list, u16 reg_list_len);

/**
 * @brief Check current regs values change
 * @param storage_pcb - pointer to storage process control block
 * @param reg_list - pointer to list of regs
 * @param reg_list_len - lenght of reg_list
 * @ingroup storage
 * @return  0 - ok,\n
 *          negative value if error,\n
 *
 * @details
 * Calculate CRC of regs values and compare with CRC in storage.
 * If change found, sets storage_pcb.data_changed flag
 */
int storage_data_changed_check(storage_pcb_t* storage_pcb, const sand_prop_save_t* reg_list, u16 reg_list_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // STORAGE_H
