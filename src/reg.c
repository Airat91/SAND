/*
 * File:        reg.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global registers access
 * Revision history: 0.1
 */

#include "reg.h"

//-------Global variables------

osMutexId regs_access_mutex = {0};

//-------Static variables------

//-------Static functions declaration-----------

//-------Functions----------

//=======Regs prop_base functions=======

sand_prop_base_t* reg_base_get_by_name(char* reg_name){
    sand_prop_base_t* reg;
    u8 reg_found = 0;

    for(u16 i = 0; i < SAND_PROP_BASE_REG_NUM; i++){
        reg = (sand_prop_base_t*)&sand_prop_base_list[i];
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

sand_prop_base_t* reg_base_get_by_ind(u16 ind){
    sand_prop_base_t* reg;

    if(ind > SAND_PROP_BASE_REG_NUM){
        reg = NULL;
    }else{
        reg = (sand_prop_base_t*)&sand_prop_base_list[ind];
    }

    return reg;
}

int reg_base_write(sand_prop_base_t* reg, u16 array_ind, reg_var_t* value){
    int result = 0;
    u16 reg_size = 0;
    u8* value_ptr = NULL;
    u8 in_storage = 0;

    if(reg == NULL){
        result = -1;
    }else{
        value_ptr = reg->p_value;
        if(reg_save_check(reg) == 0){
            in_storage = 1;
        }
        if(reg->read_only == 1){
            result = -5;
        }else if(reg_access_blocked(reg)){
            result = -2;
        }
        if(result == 0){
            // Correct min max value
            reg_range_min_max_correct(reg, value);
            reg_size = reg_base_get_byte_size(reg);
            if(array_ind <= reg->array_len){
                value_ptr += array_ind * reg_size;
                if(value->var_type == reg->type){
                    // Check storage mutex
                    if(in_storage){
                        storage_mutex_wait();
                    }
                    memcpy(value_ptr, &value->var.var_u8, reg_size);
                    if(in_storage){
                        storage_mutex_release();
                    }
                    reg_callback_exe(reg, array_ind, value);
                }else{
                    result = -4;
                }
            }else{
                result = -3;
            }
        }
    }

    return result;
}

int reg_base_read(sand_prop_base_t* reg, u16 array_ind, reg_var_t* value){
    int result = 0;

    if(reg == NULL){
        result = -1;
    }else{
        u16 byte_size = reg_base_get_byte_size(reg);
        if(reg_access_blocked(reg) == 0){
            if(array_ind <= reg->array_len){
                memcpy(&value->var.var_u8, reg->p_value + array_ind * byte_size, byte_size);
                value->var_type = reg->type;
            }else{
                result = -3;
            }
        }else{
            result = -2;
        }
    }
    if(result != 0){
        value->var.var_u64 = 0;
        value->var_type = VAR_TYPE_UNKNOWN;
    }

    return result;
}

int reg_base_get_byte_size(sand_prop_base_t* reg){
    int result = 0;
    switch(reg->type){
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

sand_header_t* reg_base_get_prop(sand_prop_base_t* reg, sand_prop_enum_t prop){
    sand_header_t* header = &reg->header;
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

//=======Regs prop_mdb functions=======

sand_prop_base_t* reg_mdb_get_by_addr(u16 addr){
    sand_prop_base_t* reg;
    sand_prop_mdb_t* property;
    u8 reg_found = 0;
    u16 start_addr = 0;
    u16 end_addr = 0;

    for(u16 i = 0; i < SAND_PROP_MDB_REG_NUM; i++){
        property = (sand_prop_mdb_t*)&sand_prop_mdb_list[i];
        reg = (sand_prop_base_t*)property->header.header_base;
        start_addr = property->mdb_addr;
        end_addr = start_addr + reg->array_len * reg_base_get_byte_size(reg)/2;
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


//=======Regs prop_range functions=======

int reg_range_min_max_correct(sand_prop_base_t* reg, reg_var_t* value){
    int result = 0;
    sand_prop_range_t* prop = NULL;

    prop = (sand_prop_range_t*)reg_base_get_prop(reg, SAND_PROP_RANGE);
    if(prop != NULL){
        // Check max limit
        if(prop->p_max != NULL){
            switch(reg->type){
            case VAR_TYPE_U8:
                if(value->var.var_u8 > *(u8*)prop->p_max){
                    value->var.var_u8 = *(u8*)prop->p_max;
                }
                break;
            case VAR_TYPE_U16:
                if(value->var.var_u16 > *(u16*)prop->p_max){
                    value->var.var_u16 = *(u16*)prop->p_max;
                }
                break;
            case VAR_TYPE_U32:
                if(value->var.var_u32 > *(u32*)prop->p_max){
                    value->var.var_u32 = *(u32*)prop->p_max;
                }
                break;
            case VAR_TYPE_U64:
                if(value->var.var_u64 > *(u64*)prop->p_max){
                    value->var.var_u64 = *(u64*)prop->p_max;
                }
                break;
            case VAR_TYPE_S8:
                if(value->var.var_s8 > *(s8*)prop->p_max){
                    value->var.var_s8 = *(s8*)prop->p_max;
                }
                break;
            case VAR_TYPE_S16:
                if(value->var.var_s16 > *(s16*)prop->p_max){
                    value->var.var_s16 = *(s16*)prop->p_max;
                }
                break;
            case VAR_TYPE_S32:
                if(value->var.var_s32 > *(s32*)prop->p_max){
                    value->var.var_s32 = *(s32*)prop->p_max;
                }
                break;
            case VAR_TYPE_S64:
                if(value->var.var_s64 > *(s64*)prop->p_max){
                    value->var.var_s64 = *(s64*)prop->p_max;
                }
                break;
            case VAR_TYPE_FLOAT:
                if(value->var.var_float > *(float*)prop->p_max){
                    value->var.var_float = *(float*)prop->p_max;
                }
                break;
            case VAR_TYPE_DOUBLE:
                if(value->var.var_double > *(double*)prop->p_max){
                    value->var.var_double = *(double*)prop->p_max;
                }
                break;
            default:
                break;
            }
        }
        // Check min limit
        if(prop->p_min != NULL){
            switch(reg->type){
            case VAR_TYPE_U8:
                if(value->var.var_u8 < *(u8*)prop->p_min){
                    value->var.var_u8 = *(u8*)prop->p_min;
                }
                break;
            case VAR_TYPE_U16:
                if(value->var.var_u16 < *(u16*)prop->p_min){
                    value->var.var_u16 = *(u16*)prop->p_min;
                }
                break;
            case VAR_TYPE_U32:
                if(value->var.var_u32 < *(u32*)prop->p_min){
                    value->var.var_u32 = *(u32*)prop->p_min;
                }
                break;
            case VAR_TYPE_U64:
                if(value->var.var_u64 < *(u64*)prop->p_min){
                    value->var.var_u64 = *(u64*)prop->p_min;
                }
                break;
            case VAR_TYPE_S8:
                if(value->var.var_s8 < *(s8*)prop->p_min){
                    value->var.var_s8 = *(s8*)prop->p_min;
                }
                break;
            case VAR_TYPE_S16:
                if(value->var.var_s16 < *(s16*)prop->p_min){
                    value->var.var_s16 = *(s16*)prop->p_min;
                }
                break;
            case VAR_TYPE_S32:
                if(value->var.var_s32 < *(s32*)prop->p_min){
                    value->var.var_s32 = *(s32*)prop->p_min;
                }
                break;
            case VAR_TYPE_S64:
                if(value->var.var_s64 < *(s64*)prop->p_min){
                    value->var.var_s64 = *(s64*)prop->p_min;
                }
                break;
            case VAR_TYPE_FLOAT:
                if(value->var.var_float < *(float*)prop->p_min){
                    value->var.var_float = *(float*)prop->p_min;
                }
                break;
            case VAR_TYPE_DOUBLE:
                if(value->var.var_double < *(double*)prop->p_min){
                    value->var.var_double = *(double*)prop->p_min;
                }
                break;
            default:
                break;
            }
        }
    }

    return result;
}

//=======Regs prop_save functions=======

int reg_save_check(sand_prop_base_t* reg){
    int result = 0;

    sand_prop_save_t* prop = (sand_prop_save_t*)reg_base_get_prop(reg, SAND_PROP_SAVE);
    if(prop == NULL){
        result = -1;
    }

    return result;
}

//=======Regs prop_access functions=======

int reg_access_blocked(sand_prop_base_t* reg){
    int result = 0;
    // Find prop_acces of reg
    sand_prop_access_t* property = (sand_prop_access_t*)reg_base_get_prop(reg, SAND_PROP_ACCESS);
    if(property != NULL){
        // Check access enable timer
        if(property->access_en_timer_ms == 0){
            result = 1;
        }
    }

    return result;
}

//=======Regs prop_callback functions=======

int reg_callback_exe(sand_prop_base_t* reg, u16 array_ind, reg_var_t* value){
    int result = 0;
    u16 val_size = reg_base_get_byte_size(reg);
    void* callback();
    // Find prop_callback of reg
    sand_prop_callback_t* property = (sand_prop_callback_t*)reg_base_get_prop(reg, SAND_PROP_CALLBACK);
    if(property != NULL){
        // Check only_end parameter
        if(property->only_end == 1){
            // Check array element is last?
            if(array_ind != (reg->array_len - 1)){
                result = -1;
            }
        }
        // Call callback function
        if(result == 0){
            property->callback();
        }
    }

    return result;
}

//-------Static functions----------



