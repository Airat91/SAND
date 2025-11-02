/*
 * File:        reg.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global registers access
 * Revision history: 0.1
 */

#include "reg.h"

//-------Global variables------

//-------Static variables------

//-------Static functions declaration-----------

static sofi_header_t* reg_base_get_prop(sofi_prop_base_t* reg, sofi_prop_enum_t prop);
static sofi_prop_base_t* reg_mdb_get_by_addr(u16 addr);
static int reg_get_byte_num(sofi_var_t);

//-------Functions----------

//=======Regs prop_base functions=======

sofi_prop_base_t* reg_base_get_by_name(char* reg_name){
    sofi_prop_base_t* reg;
    u8 reg_found = 0;

    for(u16 i = 0; i < SOFI_PROP_BASE_REG_NUM; i++){
        reg = (sofi_prop_base_t*)&sofi_prop_base_list[i];
        if(strcmp(reg->name, reg_name) == 0){
            reg_found = 1;
            break;
        }
    }
    if(reg_found == 0){
        reg = NULL;
    }

    return reg;
}

sofi_prop_base_t* reg_base_get_by_ind(u16 ind){
    sofi_prop_base_t* reg;

    if(ind > SOFI_PROP_BASE_REG_NUM){
        reg = NULL;
    }else{
        reg = (sofi_prop_base_t*)&sofi_prop_base_list[ind];
    }

    return reg;
}

int reg_base_write(sofi_prop_base_t* reg, u16 array_ind, reg_var_t* value){
    int result = 0;
    u8* value_ptr = reg->p_value;

    if(reg->read_only == 1){
        result = -1;
    }
    if(reg_access_blocked(reg)){
        result = -2;
    }
    if(result == 0){
        if(array_ind <= reg->array_len){
            value_ptr += array_ind * reg_get_byte_num(reg->type);
            if(value->var_type == reg->type){
                *value_ptr = value->var.var_u8;
            }else{
                result = -4;
            }
        }else{
            result = -3;
        }
    }

    return result;
}

reg_var_t reg_base_read(sofi_prop_base_t* reg, u16 array_ind){
    reg_var_t var = {0};
    u8* value_ptr = reg->p_value;
    var.var_type = reg->type;
    if(reg_access_blocked(reg) == 0){
        if(array_ind <= reg->array_len){
            value_ptr += array_ind * reg_get_byte_num(reg->type);
            var.var.var_u8 = *value_ptr;
        }
    }
    return var;
}

//=======Regs prop_mdb functions=======

u16 reg_mdb_read_reg(u16 addr){
    u16 result = 0;
    u16* value_ptr = 0;
    sofi_prop_base_t* reg = reg_mdb_get_by_addr(addr);
    sofi_prop_mdb_t* mdb_prop = (sofi_prop_mdb_t*)reg_base_get_prop(reg, SOFI_PROP_MDB);
    if(reg != NULL){
        value_ptr = (u16*)reg->p_value;
        // Find array element
        value_ptr += (addr - mdb_prop->mdb_addr);
        result = *value_ptr;
    }

    return result;
}

int reg_mdb_read_array(u16 addr, u16 len, u16* buf){
    int result = 0;

    return result;
}

int reg_mdb_write_reg(u16 addr, u16 value){
    int result = 0;

    return result;
}

int reg_mdb_write_array(u16 addr, u16 len, u16* buf){
    int result = 0;

    return result;
}

//=======Regs prop_range functions=======

//=======Regs prop_save functions=======

//=======Regs prop_access functions=======

int reg_access_blocked(sofi_prop_base_t* reg){
    int result = 0;
    // Find prop_acces of reg
    sofi_prop_access_t* property = (sofi_prop_access_t*)reg_base_get_prop(reg, SOFI_PROP_ACCESS);
    if(property != NULL){
        // Check access enable timer
        if(property->access_en_timer_ms == 0){
            result = 1;
        }
    }

    return result;
}

//-------Static functions----------

/**
 * @brief Get property header of register
 * @param reg - pointer to register
 * @param prop - property type
 * @ingroup reg
 * @return  - pointer to property header,\n
 *          NULL - property header not found,\n
 */
static sofi_header_t* reg_base_get_prop(sofi_prop_base_t* reg, sofi_prop_enum_t prop){
    sofi_header_t* header = &reg->header;
    u8 header_found = 0;
    for(u16 i = 0; i < reg->prop_num; i++){
        if(header->prop == prop){
            header_found = 1;
        }else{
            header = header->header_next;
        }
    }
    if(header_found == 0){
        header = NULL;
    }

    return header;
}

/**
 * @brief Find register by ModBUS address
 * @param addr - address for find
 * @ingroup reg
 * @return  - pointer to reg,\n
 *          NULL - register not found,\n
 */
static sofi_prop_base_t* reg_mdb_get_by_addr(u16 addr){
    sofi_prop_base_t* reg;
    sofi_prop_mdb_t* property;
    u8 reg_found = 0;
    u16 start_addr = 0;
    u16 end_addr = 0;

    for(u16 i = 0; i < SOFI_PROP_MDB_REG_NUM; i++){
        property = (sofi_prop_mdb_t*)&sofi_prop_mdb_list[i];
        reg = (sofi_prop_base_t*)property->header.header_base;
        start_addr = property->mdb_addr;
        end_addr = start_addr + reg->array_len * reg_get_byte_num(reg->type);
        if((addr >= start_addr)&&(addr < end_addr)){
            reg_found = 1;
            break;
        }
    }
    if(reg_found == 0){
        reg = NULL;
    }

    return reg;
}

/**
 * @brief reg_get_byte_num
 * @param var_type - var_type from sofi_var_t enumeration
 * @ingroup reg
 * @return number in bytes of var type
 */
static int reg_get_byte_num(sofi_var_t var_type){
    int result = 0;
    switch(var_type){
    case VAR_TYPE_U8:
    case VAR_TYPE_S8:
    case VAR_TYPE_CHAR:
        result = 1;
        break;
    case VAR_TYPE_U16:
    case VAR_TYPE_S16:
        result = 2;
        break;
    case VAR_TYPE_U32:
    case VAR_TYPE_S32:
    case VAR_TYPE_FLOAT:
        result = 4;
        break;
    case VAR_TYPE_U64:
    case VAR_TYPE_S64:
    case VAR_TYPE_DOUBLE:
        result = 8;
        break;
    default:
        result = 0;
    }

    return result;
}
