/*
 * File:        mdb_sand.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS API functions for SAND integration
 * Revision history: 0.1
 */

#include "mdb_sand.h"

//-------Global variables------

osThreadId mdb_sand_task_handle = {0};
mdb_sand_pcb_t mdb_sand_pcb = {0};

//-------Static variables------

static const mdb_function_t mdb_sand_suprt_fnct_list[MDB_SUPRT_FNCT_NMB] = {
    MDB_FNCT_RD_MUL_HOLD,
    MDB_FNCT_RD_MUL_INPUT,
    MDB_FNCT_WR_MUL_COIL,
    MDB_FNCT_WR_MUL_HOLD,
};

//-------Static functions declaration-----------

static void mdb_sand_gpio_init(void);
static void mdb_sand_gpio_deinit(void);
static int mdb_sand_read_reg(mdb_packet_t* packet, u8* out_buf, u16* out_len);
static int mdb_sand_write_reg(mdb_packet_t* packet, u8* out_buf, u16* out_len);
static int mdb_sand_unsupport_funct(mdb_packet_t* packet, u8* out_buf, u16* out_len);

//-------Functions----------

void mdb_sand_task(void const * argument){
    (void)argument;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){

        osDelayUntil(&last_wake_time, MDB_TASK_PERIOD);
    }
}

int mdb_sand_init(mdb_sand_pcb_t* mdb_sand_pcb){
    int result = 0;

    mdb_sand_gpio_init();
    mdb_sand_pcb->state |= MDB_ST_READY;
    debug_msg(__func__, DBG_MSG_INFO, "ModBUS inited");

    return result;
}

int mdb_deinit(mdb_sand_pcb_t* mdb_sand_pcb){
    int result = 0;

    mdb_sand_gpio_deinit();
    mdb_sand_pcb->state = MDB_ST_DISABLE;
    debug_msg(__func__, DBG_MSG_INFO, "ModBUS deinited");

    return result;
}

int mdb_sand_read_addr(void){
    int result = 0;

    if(HAL_GPIO_ReadPin(MDB_ADDR_7_PORT, MDB_ADDR_7_PIN)){
        result |= 0x01;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_6_PORT, MDB_ADDR_6_PIN)){
        result |= 0x02;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_5_PORT, MDB_ADDR_5_PIN)){
        result |= 0x04;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_4_PORT, MDB_ADDR_4_PIN)){
        result |= 0x08;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_3_PORT, MDB_ADDR_3_PIN)){
        result |= 0x10;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_2_PORT, MDB_ADDR_2_PIN)){
        result |= 0x20;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_1_PORT, MDB_ADDR_1_PIN)){
        result |= 0x40;
    }
    if(HAL_GPIO_ReadPin(MDB_ADDR_0_PORT, MDB_ADDR_0_PIN)){
        result |= 0x80;
    }

    return result;
}

int mdb_sand_packet_handle(mdb_sand_pcb_t* mdb_sand_pcb, mdb_packet_t* packet){
    int result = 0;
    u8 self_addr = mdb_sand_read_addr();
    static u8 resp_data[MDB_BUF_MAX_LEN] = {0};
    u16 resp_len = 0;

    // Reset response buf len
    mdb_sand_pcb->resp_len = 0;

    // Check address
    if(packet->slave_addr != self_addr){
        result = -1;
    }else{
        switch(packet->function){
        case MDB_FNCT_RD_MUL_HOLD:
        case MDB_FNCT_RD_MUL_INPUT:
            result = mdb_sand_read_reg(packet, resp_data, &resp_len);
            break;
        case MDB_FNCT_WR_MUL_COIL:
        case MDB_FNCT_WR_MUL_HOLD:
            result = mdb_sand_write_reg(packet, resp_data, &resp_len);
            break;
        default:
            result = mdb_sand_unsupport_funct(packet, resp_data, &resp_len);
        }
        if(result == 0){
            if(mdb_make_response(packet, resp_data, &resp_len, mdb_sand_pcb->resp_buf, &mdb_sand_pcb->resp_len) != 0){
                result = -2;
            }
        }
    }

    return result;
}

//-------Static functions----------

/**
 * @brief Init switches GPIO of ModBUS
 * @ingroup mdb
 *
 * Init MDB_ADDR switch pins
 */
static void mdb_sand_gpio_init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;

    // Init MDB_ADDR switch pins
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = MDB_ADDR_0_PIN;
    HAL_GPIO_Init (MDB_ADDR_0_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_1_PIN;
    HAL_GPIO_Init (MDB_ADDR_1_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_2_PIN;
    HAL_GPIO_Init (MDB_ADDR_2_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_3_PIN;
    HAL_GPIO_Init (MDB_ADDR_3_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_4_PIN;
    HAL_GPIO_Init (MDB_ADDR_4_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_5_PIN;
    HAL_GPIO_Init (MDB_ADDR_5_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_6_PIN;
    HAL_GPIO_Init (MDB_ADDR_6_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MDB_ADDR_7_PIN;
    HAL_GPIO_Init (MDB_ADDR_7_PORT, &GPIO_InitStruct);
}

/**
 * @brief Deinit switches GPIO of ModBUS
 * @ingroup mdb
 *
 * Deinit MDB_ADDR switch pins
 */
static void mdb_sand_gpio_deinit(void){
    HAL_GPIO_DeInit(MDB_ADDR_0_PORT, MDB_ADDR_0_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_1_PORT, MDB_ADDR_1_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_2_PORT, MDB_ADDR_2_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_3_PORT, MDB_ADDR_3_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_4_PORT, MDB_ADDR_4_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_5_PORT, MDB_ADDR_5_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_6_PORT, MDB_ADDR_6_PIN);
    HAL_GPIO_DeInit(MDB_ADDR_7_PORT, MDB_ADDR_7_PIN);
}

//=======ModBUS functions realisation=======

/**
 * @brief Read registers value
 * @param packet - pointer to input packet
 * @param out_buf - pointer to response data buffer
 * @param out_len - pointer to response data lenght
 * @ingroup mdb
 * @return 0
 *
 * @warning If register with the given address does not exist his value in response is 0
 *
 * @todo Check registers address available and send error code
 */
static int mdb_sand_read_reg(mdb_packet_t* packet, u8* out_buf, u16* out_len){
    int result = 0;
    // Vars for use
    u16 ptr = 0;
    u16 data = 0;
    u16 addr = packet->reg_addr;
    u16 array_ind = 0;
    u16 value_shift = 0;
    reg_var_t value = {0};
    sofi_prop_base_t* reg = NULL;
    sofi_prop_mdb_t* mdb_prop = NULL;
    // Vars for debug
    u16 temp = 0;
    int err = 0;

    // Add number of data bytes to response
    out_buf[ptr++] = packet->reg_nmb*2;
    // Add data bytes to response
    for(u8 i = 0; i < packet->reg_nmb; i++){
        // Get register by ModBUS address
        reg = reg_mdb_get_by_addr(addr);
        mdb_prop = (sofi_prop_mdb_t*)reg_base_get_prop(reg, SOFI_PROP_MDB);
        u16 reg_size = reg_base_get_byte_size(reg);
        if(reg != NULL){
            // Find array index and value shift for regs_size > 2
            temp = (addr - mdb_prop->mdb_addr);
            array_ind = (addr - mdb_prop->mdb_addr) * MDB_REG_BYTE_SIZE / reg_size;
            value_shift = (addr - mdb_prop->mdb_addr) % (reg_size / MDB_REG_BYTE_SIZE) * MDB_REG_BIT_SIZE;
            err = reg_base_read(reg, array_ind, &value);  // @todo: check errors
            switch(reg_size){
            case 1:
                data = value.var.var_u16;
                // Read neaxt element
                err = reg_base_read(reg, array_ind + 1, &value);  // @todo: check errors
                data += (u16)(value.var.var_u16 << 8);
                break;
            case 2:
                data = value.var.var_u16;
                break;
            case 4:
                data = (u16)(value.var.var_u32 >> value_shift);
                break;
            case 8:
                data = (u16)(value.var.var_u64 >> value_shift);
                break;
            default:
                data = 0;
            }
        }else{
            // If register with given address does not exist
            data = 0;
        }
        // Copy data to out buffer
        out_buf[ptr++] = (u8)(data >> 8);
        out_buf[ptr++] = (u8)data;
        // Increase ModBUS register address
        addr++;
    }
    *out_len = ptr;

    return result;
}

static int mdb_sand_write_reg(mdb_packet_t* packet, u8* out_buf, u16* out_len){
    int result = 0;

    // Write values from packet
    u16 ptr = 0;
    // For debug only
    // Copy reg address to response
    out_buf[ptr++] = (u8)packet->reg_addr >> 8;
    out_buf[ptr++] = (u8)packet->reg_addr;
    // Cop regs number to response
    out_buf[ptr++] = (u8)packet->reg_nmb >> 8;
    out_buf[ptr++] = (u8)packet->reg_nmb;
    *out_len = ptr;

    return result;
}

/**
 * @brief Write error value to output buffer
 * @param packet - pointer to input packet
 * @param out_buf - pointer to output buffer
 * @param out_len - pointer to output buffer lenght
 * @ingroup mdb
 * @return 0
 */
static int mdb_sand_unsupport_funct(mdb_packet_t* packet, u8* out_buf, u16* out_len){
    int result = 0;

    packet->function |= MDB_FNCT_ERR_FLAG;
    out_buf[0] = MDB_ERR_FUNCT;
    *out_len = 1;

    return result;
}
