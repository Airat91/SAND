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
    u32 addr_end = addr + len * 2;

    if((addr < FLASH_START) || (addr > FLASH_END)){
        result = -1;
        // Addr not in FLASH area
    }else if(addr_end > FLASH_END){
        // End of read data out of FLASH area
        result = -2;
    }else{
        // @note memcpy() use lenght in bytes, terefore multiple len * 2
        memcpy(buf, (u32*)addr, len * 2);
    }

    return result;
}

int flash_write(u32 addr, u16* buf, u16 len){
    int result = 0;
    u16 ptr = 0;
    u32 addr_end = addr + len * 2;
    flash_write_size_t write_size = 0;
    HAL_StatusTypeDef stat = HAL_OK;

    // Check addr
    if(addr%2 == 1){
        result = -1;
        // Address is not even
    }else if(addr_end > FLASH_END){
        // End of read data out of FLASH area
        result = -2;
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

    // @note memcpy() use lenght in bytes, terefore multiple len * 2
    memcpy(buf, (u32*)addr, len * 2);

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
