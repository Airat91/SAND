/*
 * File:        flash.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Functions for work with internal FLASH
 * Revision history: 0.1
 */

#include "flash.h"

//-------Global variables------

//-------Static variables------

//-------Static functions declaration-----------

static void flash_htol(u16* word);

//-------Functions----------

int flash_read(u32 addr, u16* buf, u16 len){
    int result = 0;
    u32* ptr = (u32*)addr;
    u32 addr_end = addr + len;

    if((addr < FLASH_START) || (addr > FLASH_END)){
        result = -1;
        // Addr not in FLASH area
    }else{
        if(addr_end > FLASH_END){
            result = -2;
            // End of read data out of FLASH area
        }else{
            // @note memcpy() use lenght in bytes, terefore multiple len * 2
            memcpy(buf, ptr, len * 2);
        }
    }

    return result;
}

int flash_write(u32 addr, u16* buf, u16 len){
    int result = 0;
    u16 ptr = 0;
    flash_write_size_t write_size = 0;
    HAL_StatusTypeDef stat = HAL_OK;

    // Check addr
    if(addr%2 == 1){
        result = -1;
        // Address is not even
    }else{
        // Unlock FLASH before write
        HAL_FLASH_Unlock();

        // Don't use while-loop in handling cycle
        for(u16 i = 0; i < len; i++){
            // Get tail size of buffer in half-words (16 bit)
            write_size = len - ptr;
            // Select one of writing mode
            if(write_size >= FLASH_WRITE_DOUBLEWORD){
                write_size = FLASH_WRITE_DOUBLEWORD;
            }else if(write_size >= FLASH_WRITE_WORD){
                write_size = FLASH_WRITE_WORD;
            }else{
                write_size = FLASH_WRITE_HALFWORD; // Half-word (16-bit)
            }
            switch(write_size){
            case FLASH_WRITE_HALFWORD:
                stat = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, *(u16*)buf);
                break;
            case FLASH_WRITE_WORD:
                stat = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, *(u32*)buf);
                break;
            case FLASH_WRITE_DOUBLEWORD:
                stat = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, *(u64*)buf);
                break;
            }
            if(stat != HAL_OK){
                result = -2;
                debug_msg(__func__, DBG_MSG_ERR, "HAL_FLASH_Program(addr = 0x%08X) %S", addr, hal_status[stat]);
                break;
            }
            // Increase address and data pointer
            buf += write_size;
            ptr += write_size;
            addr += write_size * 2;
            if(ptr >= len){
                break;
            }
        }

        // Lock FLASH after write
        HAL_FLASH_Lock();
    }

    return result;
}

int flash_read_global(u32 addr, u16* buf, u16 len){
    int result = 0;

    u32* ptr = (u32*)addr;
    // @note memcpy() use lenght in bytes, terefore multiple len * 2
    memcpy(buf, ptr, len * 2);

    return result;
}

//-------Static functions----------

/**
 * @brief Swap High and Low bytes into 16-bit word
 * @param word - pointer to 16-bit word for change
 * @ingroup flash
 */
static void flash_htol(u16* word){
    u16 temp = 0;
    temp  = (u16)(*word & 0x00FF << 8);
    temp |= (u16)(*word & 0xFF00 >> 8);
    *word = temp;
}



//-------Unrefactoried----------

#include "flash.h"
#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal.h"

/**
  * @defgroup flash
  * @brief work with flash
  */


/*========= GLOBAL VARIABLES ==========*/



/*========== FUNCTIONS ==========*/

/**
 * @brief save_to_flash
 * @param area_cnt
 * @param start_position
 * @param data
 * @return
 */
int save_to_flash(int area_cnt, uint8_t start_position, uint16_t *data){
    int result = 0;
    //int area_cnt = find_free_area();
    //uint64_t * pbuf = (void*)data;
    uint32_t area_addr = FLASH_SAVE_PAGE_ADDRESS + (uint32_t)area_cnt*SAVE_AREA_SIZE;;
    uint32_t cur_addr = area_addr + start_position*2U;

    if(0){//area_cnt < find_free_area()){
        result = -1;
    }else{
        HAL_FLASH_Unlock();
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, cur_addr, *data) != HAL_OK){
            result = -2;
        }
        HAL_FLASH_Lock();
    }

    return result;
}

/**
 * @brief Find free area in page to save
 * @return  -1 - page is fill, \n
 *          area number
 * @ingroup flash
 */
int find_free_area(void){
    int result = -1;
    uint8_t area_cnt = 0;
    uint8_t read = 0xFF;
    uint8_t *addr = (uint8_t*)FLASH_SAVE_PAGE_ADDRESS;
    for(area_cnt = 0; area_cnt < SAVE_AREA_NMB; area_cnt++){
        read = 0xFF;
        for(uint8_t i = 0; i < SAVE_AREA_SIZE; i++){
            read &= *addr;
            addr++;
        }
        if(read == 0xFF){
            result = area_cnt;
            break;
        }
    }

    return result;
}
