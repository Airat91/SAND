/*
 * File:        flash.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Functions for work with internal FLASH
 * Revision history: 0.1
 */

#ifndef FLASH_H
#define FLASH_H 1

/*add includes below */
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"
#include "stdint.h"
#include "string.h"
#include "debug.h"
/*add includes before */

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @defgroup flash
 * @brief Functions for work with internal FLASH
 */

//--------Defines--------

#if(STM32F103xB)    // STM32F103CBT6
    #define FLASH_TOTAL_SIZE        0x00020000  // 128 Kbytes
    #define FLASH_PAGE_SIZE         0x00000400  // 1 Kbytes
#elif(STM32F103x8)  // STM32F103C8T6
    #define FLASH_TOTAL_SIZE        0x00010000  // 64 Kbytes
    #define FLASH_PAGE_SIZE         0x00000400  // 1 Kbytes
#elif(K1986BE92FI)  // K1986BE92FI (Milandr)
    #define FLASH_TOTAL_SIZE        0x0020000   // 128 Kbytes
    #define FLASH_PAGE_SIZE         0x0001000   // 4K bytes
#endif // MCU_target

#define FLASH_START                 0x08000000  // Start address of FLASH
#define FLASH_END                   FLASH_START + FLASH_TOTAL_SIZE
#define FLASH_PAGE_NMB              FLASH_TOTAL_SIZE / FLASH_PAGE_SIZE

// Internal signature addresses
#define FLASH_SIG_SIZE_ADDR         0x1FFFF7E0  // Flash size register address in FLASH
#define FLASH_SIG_SIZE_BYTE_LEN     2           // Flash size register lenght in bytes
#define FLASH_SIG_ID_ADDR           0x1FFFF7E8  // Unique ID informarion address in FLASH
#define FLASH_SIG_ID_BYTE_LEN       12          // Unique ID lenght in bytes

//--------Typedefs-------

typedef enum{
    FLASH_WRITE_HALFWORD    = 1,    // Half-word (16-bit)
    FLASH_WRITE_WORD        = 2,    // Word (32 bit)
    FLASH_WRITE_DOUBLEWORD     = 4,    // Double-word (64 bit)
}flash_write_size_t;

//-------External variables------

//-------Function prototypes----------

/**
 * @brief Read data from device FLASH memory
 * @param addr - global address of FLASH
 * @param buf - pointer for read
 * @param len - data lenght in 16-bit words
 * @ingroup flash
 * @return  0 - ok,\n
 *          -1 - Address not in FLASH area,\n
 *          -2 - End of read data out of FLASH area,\n
 */
int flash_read(u32 addr, u16* buf, u16 len);

/**
 * @brief Write data into device FLASH memory
 * @param addr - global address of FLASH
 * @param buf - pointer of data
 * @param len - data lenght in 16-bit words
 * @ingroup flash
 * @return  0 - ok,\n
 *          -1 - Address is not even,\n
 *          -2 - End of read data out of FLASH area,\n
 *
 * @warning Global address of FLASH must be even
 */
int flash_write(u32 addr, u16* buf, u16 len);

/**
 * @brief Read data from global memory
 * @param addr - global address
 * @param buf - pointer for read
 * @param len - data lenght in 16-bit words
 * @ingroup flash
 * @return  0
 */
int flash_read_global(u32 addr, u16* buf, u16 len);

#ifdef __cplusplus
}
#endif

#endif // FLASH_H
