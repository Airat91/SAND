/*
 * File:        storage.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Work with sand storage
 * Revision history: 0.1
 */

#include "storage.h"

//-------Global variables------

storage_pcb_t storage_pcb = {0};
osMutexId regs_storage_mutex = {0};

//-------Static variables------

static CRC_HandleTypeDef hcrc = {0};
static storage_fifo_pcb_t storage_fifo_pcb = {0};   // Storage FIFO process control block

//-------Static functions declaration-----------

static int storage_crc_init(CRC_HandleTypeDef* hcrc);
static u32 storage_calc_names_crc(const sand_prop_save_t* reg_list, u16 reg_list_len);
static u32 storage_calc_data_crc(const sand_prop_save_t* reg_list, u16 reg_list_len);
static storage_dump_t* storage_dump_find(void);
static int storage_erase(u32* erase_cnt);
static int storage_dump_data_validation(storage_dump_t* dump);
static int storage_fifo_init(storage_fifo_pcb_t* storage_fifo, u32 flash_addr);
static int storage_fifo_push(storage_fifo_pcb_t* storage_fifo, u8* data, u16 len);
static int storage_fifo_write_tail(storage_fifo_pcb_t* storage_fifo);

//-------Functions----------

int storage_init(storage_pcb_t* storage_pcb){
    int result = 0;
    if(storage_crc_init(&hcrc) != 0){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "storage_crc_init() error");
    }
    // Reset storage_pcb
    memset(storage_pcb, 0, sizeof(storage_pcb_t));
    // Calculate actual names_crc
    storage_pcb->current_names_crc = storage_calc_names_crc(&sand_prop_save_list[0], SAND_PROP_SAVE_REG_NUM);
    // Get last dump from storage
    storage_pcb->dump = storage_dump_find();
    if(storage_pcb->dump != NULL){
        storage_pcb->erase_cnt = storage_pcb->dump->header.erase_cnt;
    }else{
        debug_msg(__func__, DBG_MSG_WARN, "Dump not foud");
    }

    return result;
}

int storage_restore_data(storage_pcb_t* storage_pcb){
    int result = 0;

    sand_prop_base_t* reg = NULL;
    u16 bytes_nmb = 0;
    u32 addr = 0;
    u16 regs_cnt = 0;

    // Take regs_storage_mutex
    storage_mutex_wait();

    if(storage_pcb->dump != NULL){
        // If dump is exist, validate his data
        if(storage_dump_data_validation(storage_pcb->dump) == 0){
            // Read registers number in dump and compare with registers number in project
            if(storage_pcb->dump->header.regs_num <= SAND_PROP_SAVE_REG_NUM){

            }

            // Compare names_crc
            if(storage_pcb->current_names_crc == storage_pcb->dump->header.names_crc){
                // Restore last values from dump
                for(u16 i = 0; i < SAND_PROP_SAVE_REG_NUM; i++){
                    reg = (sand_prop_base_t*)sand_prop_save_list[i].header.header_base;
                    bytes_nmb = reg_base_get_byte_size(reg) * reg->array_len;
                    addr = (u32)&storage_pcb->dump->data + sand_prop_save_list[i].save_addr;
                    flash_read(addr, (u16*)reg->p_value, bytes_nmb);
                }
                // Update data_crc from dump
                storage_pcb->current_data_crc = storage_pcb->dump->header.data_crc;

                // Reset data changed flag
                storage_pcb->data_changed = 0;
            }else{
                result = -3;
                debug_msg(__func__, DBG_MSG_WARN, "Dump names_crc mismatch");
            }
        }else{
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "Dump data is not valid");
        }
    }else{
        result = -1;
    }

    if(result != 0){
        // If dump doesn't exist or not valid, set default values or null
        storage_set_defaults(storage_pcb);
        debug_msg(__func__, DBG_MSG_WARN, "Data can't be restored, setting default values");

        // Set data changed flag
        storage_pcb->data_changed = 1;
    }


    // Release regs_storage_mutex
    storage_mutex_release();

    return result;
}

int storage_save_data(storage_pcb_t* storage_pcb){
    int result = 0;

    sand_prop_base_t* reg = NULL;
    storage_header_t dump_header = {0};
    storage_dump_t* saved_dump = NULL;
    const u16 dump_size = SAND_SAVE_DATA_SIZE + sizeof(storage_header_t);
    u16 reg_size = 0;       // Register size in bytes
    u32 flash_addr = 0;     // Address value for save into storage FLASH
    u16 reg_nmb = 0;        // Register counter for reg_list
    u16 save_addr_ptr = 0;  // Save address index
    u8 null_value = 0;      // Null value for fill unused addresses

    // Take regs_storage_mutex
    storage_mutex_wait();

    // Fill new_dump header
    dump_header.data_crc = storage_calc_data_crc(&sand_prop_save_list[0], SAND_PROP_SAVE_REG_NUM);
    dump_header.names_crc = storage_pcb->current_names_crc;
    dump_header.regs_num = SAND_PROP_SAVE_REG_NUM;
    dump_header.data_len = SAND_SAVE_DATA_SIZE;
    // Check available storage empty area for new dump
    if(storage_pcb->dump != NULL){
        if((u32)storage_pcb->dump->header.next_header + dump_size > STORAGE_FLASH_END){
            // If new dump doesn't fit in free area, erase storage FLASH and write from STORAGE_FLASH_START
            flash_addr = STORAGE_FLASH_START;                                   // Start address of new dump
            dump_header.next_header = &*(u32*)(STORAGE_FLASH_START + dump_size);// Set pointer address value
            dump_header.erase_cnt = storage_pcb->erase_cnt;
            storage_erase(&dump_header.erase_cnt);                              // Erase storage FLASH and increase erase counter
        }else{
            // Write from dump.next_header address
            flash_addr = (u32)storage_pcb->dump->header.next_header;            // Start address of new dump
            dump_header.next_header = &*(u32*)(flash_addr + dump_size);         // Set pointer address value
            dump_header.erase_cnt = storage_pcb->erase_cnt;
        }
    }else{
        // Write from STORAGE_FLASH_START
        flash_addr = STORAGE_FLASH_START;                                       // Start address of new dump
        dump_header.next_header = &*(u32*)(STORAGE_FLASH_START + dump_size);    // Set pointer address value
        dump_header.erase_cnt = storage_pcb->erase_cnt;
    }

    // Init storage FIFO
    storage_fifo_init(&storage_fifo_pcb, flash_addr);

    // Push dump_header to FIFO
    storage_fifo_push(&storage_fifo_pcb, (u8*)&dump_header, sizeof(storage_header_t));

    // Push data to FIFO
    save_addr_ptr = 0;
    for(reg_nmb = 0; reg_nmb < SAND_PROP_SAVE_REG_NUM; reg_nmb++){
        reg = (sand_prop_base_t*)reg_base_get_prop(sand_prop_save_list[reg_nmb].header.header_base, SAND_PROP_BASE);
        reg_size = reg_base_get_byte_size(reg) * reg->array_len;
        if(sand_prop_save_list[reg_nmb].save_addr > save_addr_ptr){
            // Fill unused addresses by nulls
            while(sand_prop_save_list[reg_nmb].save_addr > save_addr_ptr){
                storage_fifo_push(&storage_fifo_pcb, &null_value, 1);
                save_addr_ptr++;
            }
        }
        // Push reg value into storage
        storage_fifo_push(&storage_fifo_pcb, reg->p_value, reg_size);
        save_addr_ptr += reg_size;
    }
    // Write tail of FIFO
    storage_fifo_write_tail(&storage_fifo_pcb);

    // Release regs_storage_mutex
    storage_mutex_release();

    // Update storage_pcb
    storage_pcb->current_data_crc = dump_header.data_crc;
    storage_pcb->erase_cnt = dump_header.erase_cnt;
    storage_pcb->data_changed = 0;
    storage_pcb->dump = &*(storage_dump_t*)flash_addr;
    storage_pcb->last_save_time_ms = HAL_GetTick();

    // Validate saved dump
    saved_dump = (storage_dump_t*)flash_addr;
    if(saved_dump->header.next_header != dump_header.next_header){
        // Fatal error. Need full STORAGE_FLASH erasing and reinit storage
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "Saved dump next_header is not valid, need full storage FLASH erasing");
    }else{
        // Validate saved dump header elements
        if(saved_dump->header.data_crc != dump_header.data_crc){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "Saved dump data_crc is not valid, need resave dump");
        }
        if(saved_dump->header.names_crc != dump_header.names_crc){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "Saved dump names_crc is not valid, need resave dump");
        }
        if(saved_dump->header.data_len != dump_header.data_len){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "Saved dump data_len is not valid, need resave dump");
        }
        if(saved_dump->header.regs_num != dump_header.regs_num){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "Saved dump regs_num is not valid, need resave dump");
        }
        if(saved_dump->header.erase_cnt != dump_header.erase_cnt){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "Saved dump erase_cnt is not valid, need resave dump");
        }
        if(result == 0){
            // Validate data of saved dump
            if(storage_dump_data_validation(saved_dump) != 0){
                result = -3;
                debug_msg(__func__, DBG_MSG_ERR, "Saved dump data is not valid, need resave dump");
            }
        }
    }

    return result;
}

int storage_data_changed_check(storage_pcb_t* storage_pcb){
    int result = 0;

    u32 crc = storage_calc_data_crc(&sand_prop_save_list[0], SAND_PROP_SAVE_REG_NUM);
    if(crc != storage_pcb->current_data_crc){
        storage_pcb->data_changed = 1;
    }

    return result;
}

void storage_mutex_wait(void){
    if(regs_storage_mutex != NULL){
        osMutexWait(regs_storage_mutex, STORAGE_MUTEX_TIMEOUT);
    }
}

void storage_mutex_release(void){
    if(regs_storage_mutex != NULL){
        osMutexRelease(regs_storage_mutex);
    }
}

int storage_handle(storage_pcb_t* storage_pcb, u16 period_ms){
    int result = 0;
    static u16 tick = 0;

    // Check data change every call
    storage_data_changed_check(storage_pcb);
    if(tick / 1000 >= storage.vars.autosave_period){
        // Check autosave flag
        if(storage.vars.autosave_en == 1){
            if(storage_pcb->data_changed == 1){
                result = storage_save_data(storage_pcb);
            }
        }
        switch(result){
        case -1:
            // Saved dump next_header is not valid, need full storage FLASH erasing
            storage_erase(&storage_pcb->erase_cnt);
            storage_pcb->data_changed = 1;
            storage_pcb->dump = NULL;
            tick = storage.vars.autosave_period * 1000 - STORAGE_RESAVE_PERIOD_MS;// / period_ms;
            break;
        case -2:
        case -3:
            // Saved dump is not valid, need resave dump
            storage_pcb->data_changed = 1;
            tick = storage.vars.autosave_period * 1000 - STORAGE_RESAVE_PERIOD_MS;// / period_ms;
            break;
        default:
            tick = 0;
        }
    }

    // Write storage parameters to storage.vars every call
    storage.vars.erase_cnt      = storage_pcb->erase_cnt;
    storage.vars.last_save_time = storage_pcb->last_save_time_ms;
    storage.vars.dump_addr      = (u32)storage_pcb->dump;
    storage.vars.dump_size      = sizeof(storage_header_t) + SAND_SAVE_DATA_SIZE;
    storage.vars.data_changed   = storage_pcb->data_changed;

    tick += period_ms;

    return result;
}

int  storage_set_defaults(storage_pcb_t* storage_pcb){
    int result = 0;

    sand_prop_base_t* reg = NULL;
    sand_prop_range_t* prop_range = NULL;
    u16 bytes_nmb = 0;

    for(u16 i = 0; i < SAND_PROP_SAVE_REG_NUM; i++){
        reg = (sand_prop_base_t*)sand_prop_save_list[i].header.header_base;
        prop_range = (sand_prop_range_t*)reg_base_get_prop(reg, SAND_PROP_RANGE);
        if(reg->type == VAR_TYPE_CHAR){
            // Get length of def-string
            bytes_nmb = strlen(prop_range->p_def);
        }else{
            // Get length in bytes
            bytes_nmb = reg_base_get_byte_size(reg) * reg->array_len;
        }
        if(prop_range != NULL){
            // Copy default value into register value
            memcpy(reg->p_value, prop_range->p_def, bytes_nmb);
        }else{
            // Write nulls
            memset(reg->p_value, 0, bytes_nmb);
        }
    }

    return result;
}

//-------Static functions----------

/**
 * @brief Init HardWare CRC calculation unit
 * @param hcrc - pointer to CRC_HandleTypeDef of CRC
 * @ingroup storage
 * @return 0
 */
static int storage_crc_init(CRC_HandleTypeDef* hcrc){
    int result = 0;

    __HAL_RCC_CRC_CLK_ENABLE();

    hcrc->Instance = CRC;
    HAL_CRC_Init(hcrc);
    __HAL_CRC_DR_RESET(hcrc);

    return result;
}

/**
 * @brief Calculate CRC of registers names from list
 * @param reg_list - pointer to list
 * @param reg_list_len - regs list length
 * @ingroup storage
 * @return CRC of registers names from list
 */
static u32 storage_calc_names_crc(const sand_prop_save_t* reg_list, u16 reg_list_len){
    u32 crc = 0;
    u16 name_len = 0;
    u8* char_ptr = NULL;

    sand_prop_base_t* reg = NULL;
    // Reset CRC hardware
    __HAL_CRC_DR_RESET(&hcrc);
    for(u16 i = 0; i < reg_list_len; i++){
        // Get register from list
        reg = (sand_prop_base_t*)reg_list[i].header.header_base;
        name_len = strlen(reg->name);
        char_ptr = (u8*)reg->name;
        for(u16 ptr = 0; ptr < name_len; ptr++){
            // Add symbol to CRC data register
            hcrc.Instance->DR = *char_ptr++;
        }
    }
    crc = hcrc.Instance->DR;

    return crc;
}

/**
 * @brief Calculate CRC of registers values from list
 * @param reg_list - pointer to list
 * @param reg_list_len - regs list length
 * @ingroup storage
 * @return CRC of registers values from list
 */
static u32 storage_calc_data_crc(const sand_prop_save_t* reg_list, u16 reg_list_len){
    u32 crc = 0;
    u16 bytes_nmb = 0;
    u8* val_ptr = NULL;
    u16 data_len = 0;

    sand_prop_base_t* reg = NULL;
    // Reset CRC hardware
    __HAL_CRC_DR_RESET(&hcrc);
    for(u16 i = 0; i < reg_list_len; i++){
        // Get register from list
        reg = (sand_prop_base_t*)reg_list[i].header.header_base;
        bytes_nmb = reg_base_get_byte_size(reg) * reg->array_len;
        val_ptr = reg->p_value;
        for(u16 ptr = 0; ptr < bytes_nmb; ptr++){
            // Add value byte to CRC data register
            hcrc.Instance->DR = (u8)*val_ptr++;
            data_len++;
        }
    }
    crc = hcrc.Instance->DR;

    return crc;
}

/**
 * @brief Find last dump in storage
 * @ingroup storage
 * @return  - pointer to dump,\n
 *          NULL - dump not found,\n
 */
static storage_dump_t* storage_dump_find(void){
    storage_dump_t* dump = NULL;
    u32 addr = STORAGE_FLASH_START;
    while(*(u32*)addr != 0xFFFFFFFF){
        dump = (storage_dump_t*)addr;
        addr = (u32)dump->header.next_header;
        if(addr >= STORAGE_FLASH_END){
            break;
        }
        if(addr < STORAGE_FLASH_START){
            // Addr is broken
            dump = NULL;
            break;
        }
    }

    return dump;
}

/**
 * @brief Erase storage FLASH with read number of erases from last dump
 * @param erase_cnt - pointer to erase counter in
 * @return  0 - ok,\n
 *          -1 Erase error,\n
 */
static int storage_erase(u32* erase_cnt){
    int result = 0;

    storage_dump_t* dump = storage_dump_find();
    if(dump != NULL){
        *erase_cnt = dump->header.erase_cnt + 1;
    }

    FLASH_EraseInitTypeDef erase = {0};
    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.Banks = FLASH_BANK_1;
    erase.PageAddress = STORAGE_FLASH_START;
    erase.NbPages = STORAGE_FLASH_PAGE_NMB;
    u32 page_error = 0;

    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&erase, (uint32_t*)&page_error);
    HAL_FLASH_Lock();

    if(page_error != 0xFFFFFFFF){
        result = -1;
    }

    return result;
}

/**
 * @brief Validate data in dump
 * @param dump - pointer to dump for validate
 * @ingroup storage
 * @return  0 - ok,\n
 *          -1 - dump pointer is NULL,\n
 *          -2 - data_crc mismatch,\n
 */
static int storage_dump_data_validation(storage_dump_t* dump){
    int result = 0;

    u8* val_ptr = NULL;
    u32 crc_calc = 0;
    u16 data_len = 0;
    if(dump != NULL){
        // Reset CRC hardware
        __HAL_CRC_DR_RESET(&hcrc);
        val_ptr = &dump->data;
        data_len = dump->header.data_len;
        for(u16 i = 0; i < data_len ; i++){
            // Add value byte to CRC data register
            hcrc.Instance->DR = (u8)*val_ptr++;
        }
        crc_calc = hcrc.Instance->DR;
        if(crc_calc != dump->header.data_crc){
            // data_crc mismatch
            result = -2;
        }
    }else{
        // Dump pointer is NULL
        result = -1;
    }

    return result;
}

/**
 * @brief Init storage_fifo buffer and reset buf_ptr
 * @param storage_fifo - pointer to FIFO control block
 * @param flash_addr - start flash address for data writing
 * @ingroup storage
 * @return  0
 */
static int storage_fifo_init(storage_fifo_pcb_t* storage_fifo_pcb, u32 flash_addr){
    int result = 0;

    memset(storage_fifo_pcb->buf, 0, STORAGE_SAVE_DATA_BUF_LEN);
    storage_fifo_pcb->buf_ptr = 0;
    storage_fifo_pcb->addr_ptr = flash_addr;
    storage_fifo_pcb->flash_write_cnt = 0;

    return result;
}

/**
 * @brief Write data to FIFO buffer for writing into storage FLASH
 * @param storage_fifo_pcb - pointer to FIFO control block
 * @param data - pointer to data for writing
 * @param len - data length in bytes
 * @ingroup storage
 * @return  0
 */
static int storage_fifo_push(storage_fifo_pcb_t* storage_fifo_pcb, u8* data, u16 len){
    int result = 0;

    u16 write_size = STORAGE_SAVE_DATA_BUF_LEN - storage_fifo_pcb->buf_ptr;
    // Fill buffer and write it into FLASH
    while(len >= write_size){
        memcpy(&storage_fifo_pcb->buf[storage_fifo_pcb->buf_ptr], data, write_size);
        flash_write(storage_fifo_pcb->addr_ptr, (u16*)storage_fifo_pcb->buf, STORAGE_SAVE_DATA_BUF_LEN/2);
        storage_fifo_pcb->flash_write_cnt++;
        storage_fifo_pcb->addr_ptr += STORAGE_SAVE_DATA_BUF_LEN;
        len -= write_size;
        data += write_size;
        storage_fifo_pcb->buf_ptr = 0;
        write_size = STORAGE_SAVE_DATA_BUF_LEN - storage_fifo_pcb->buf_ptr;
    }
    // Write tail of data to buffer
    if(len > 0){
        memcpy(&storage_fifo_pcb->buf[storage_fifo_pcb->buf_ptr], data, len);
        storage_fifo_pcb->buf_ptr += len;
    }

    return result;
}

/**
 * @brief Write tail of FIFO buffer into storage FLASH
 * @param storage_fifo_pcb - pointer to FIFO control block
 * @ingroup storage
 * @return  0
 *
 * @note If tail length is odd, the null adds
 */
static int storage_fifo_write_tail(storage_fifo_pcb_t* storage_fifo_pcb){
    int result = 0;

    if(storage_fifo_pcb->buf_ptr % 2 == 1){
        storage_fifo_pcb->buf[storage_fifo_pcb->buf_ptr++] = 0;
    }
    flash_write(storage_fifo_pcb->addr_ptr, (u16*)storage_fifo_pcb->buf, storage_fifo_pcb->buf_ptr/2);
    storage_fifo_pcb->flash_write_cnt++;
    storage_fifo_pcb->addr_ptr += storage_fifo_pcb->buf_ptr;
    storage_fifo_pcb->buf_ptr = 0;

    return result;
}
