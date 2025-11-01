/*
 * File:        modbus.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS API functions for SAND integration
 * Revision history: 0.1
 */

#include "modbus.h"

//-------Global variables------

//-------Static variables------

//-------Static functions declaration-----------

static int mdb_function_correct(u8 code);
static u16 mdb_crc16_calc(u8* buf, u16 len);

//-------Functions----------

mdb_packet_t mdb_packet_recognise(u8* buf, u16 len){
    mdb_packet_t packet = {0};
    u8 mismatch = 0;

    // Check for ModBUS-ASCII
    if(packet.protocol == MDB_PROT_UNKNOWN){
        mismatch = 0;
        if(buf[0] != MDB_ASCII_START){
            mismatch++;
        }

        if(mismatch == 0){
            packet.protocol = MDB_PROT_ASCII;
        }else{
            // reset packet struct
            memset(&packet, 0, sizeof(mdb_packet_t));
        }
    }

    // Check for ModBUS-RTU
    if(packet.protocol == MDB_PROT_UNKNOWN){
        mismatch = 0;
        packet.slave_addr = buf[0];
        packet.function = buf[1];
        packet.reg_addr = ((u16)buf[2] << 8);
        packet.reg_addr += (u16)buf[3];
        packet.reg_nmb = ((u16)buf[4] << 8);
        packet.reg_nmb += (u16)buf[5];
        packet.data = &buf[6];
        packet.crc = ((u16)buf[len - 1] << 8);
        packet.crc += (u16)buf[len - 2];

        if(mdb_function_correct(packet.function) == 0){
            mismatch++;
        }
        if(mdb_crc16_calc(buf, len-2) != packet.crc){
            mismatch++;
        }

        if(mismatch == 0){
            packet.protocol = MDB_PROT_RTU;
        }else{
            // reset packet struct
            memset(&packet, 0, sizeof(mdb_packet_t));
        }
    }

    return packet;
}

int mdb_make_response(mdb_packet_t* packet, u8* data, u16* data_len, u8* out_buf, u16* out_len){
    int result = 0;
    u16 ptr = 0;
    u16 crc16 = 0;

    // Make packet for ModBUS-RTU
    if(packet->protocol == MDB_PROT_RTU){
        out_buf[ptr++] = packet->slave_addr;
        out_buf[ptr++] = packet->function;
        memcpy(&out_buf[ptr], data, *data_len);
        ptr += *data_len;
        crc16 = mdb_crc16_calc(out_buf, ptr);
        memcpy(&out_buf[ptr], &crc16, sizeof(crc16));
        ptr += sizeof(crc16);
        *out_len = ptr;
    }

    if(packet->protocol == MDB_PROT_ASCII){

    }

    return result;
}

//-------Static functions----------

/**
 * @brief Check function code in mdb_function_t enumeration
 * @param code - function code for checking
 * @ingroup mdb
 * @return  0 - incorrect,\n
 *          1 - correct,\n
 */
static int mdb_function_correct(u8 code){
    int result = 0;

    switch(code){
    case MDB_FNCT_RD_MUL_COIL:
    case MDB_FNCT_RD_MUL_DISCR:
    case MDB_FNCT_RD_MUL_HOLD:
    case MDB_FNCT_RD_MUL_INPUT:
    case MDB_FNCT_WR_SIN_COIL:
    case MDB_FNCT_WR_SIN_HOLD:
    case MDB_FNCT_RD_EXCP_STAT:
    case MDB_FNCT_DIAGNOSTIC:
    case MDB_FNCT_RD_EVENT_CNT:
    case MDB_FNCT_RD_EVENT_LOG:
    case MDB_FNCT_WR_MUL_COIL:
    case MDB_FNCT_WR_MUL_HOLD:
    case MDB_FNCT_RD_FILE:
    case MDB_FNCT_WR_FILE:
    case MDB_FNCT_WR_MASK_REG:
    case MDB_FNCT_RD_WR_MUL_REGS:
    case MDB_FNCT_RD_FIFO:
        result = 1;
        break;
    default:
        result = 0;
    }

    return result;
}

/**
 * @brief Calculate CRC16 for ModBUS packet
 * @param buf - pointer to ModBUS packet
 * @param len - packet lenght without CRC
 * @ingroup mdb
 * @return calculated crc16 for packet
 */
static u16 mdb_crc16_calc(u8* buf, u16 len){
    u16 crc;

    u16 i, j;
    len = len > 254?254:len;
    crc = 0xFFFF;
    for (i = 0; i < len; i++)  {
        crc ^= buf[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x01) == 1){
                crc = (crc >> 1) ^ 0xA001;
            }else{
                crc >>= 1;
            }
        }
    }

    return crc;
}
