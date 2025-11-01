/*
 * File:        modbus.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS API functions for SAND integration
 * Revision history: 0.1
 */

#include "modbus.h"

//-------Global variables------

//-------Static variables------

static u8 mdb_buf_temp[MDB_BUF_MAX_LEN] = {0};  // Temp buffer for ASCII convertion

//-------Static functions declaration-----------

static int mdb_function_correct(u8 code);
static u16 mdb_crc16_calc(u8* buf, u16 len);
static u8 mdb_lrc8_calc(u8* buf, u16 len);
static u8 mdb_ascii_to_byte(u16 ascii);
static u8 mdb_halfbyte_to_ascii(u8 byte);

//-------Functions----------

mdb_packet_t mdb_packet_recognise(u8* buf, u16 len){
    mdb_packet_t packet = {0};
    u8 mismatch = 0;
    u16 ptr = 0;

    // Check for ModBUS-ASCII
    if(packet.protocol == MDB_PROT_UNKNOWN){
        mismatch = 0;
        if(buf[0] != MDB_ASCII_START){
            mismatch++;
        }
        if(buf[len - 2] != MDB_ASCII_PREEND){
            mismatch++;
        }
        if(buf[len - 1] != MDB_ASCII_END){
            mismatch++;
        }
        if(len%2 != 1){
            mismatch++;
        }
        if(mismatch == 0){
            // Decode ASCII to temp buffer
            ptr = 0;
            u8 ind = 1;
            for(u8 i = 1; i < len - 2; i += 2){
                mdb_buf_temp[ptr] = mdb_ascii_to_byte(buf[i]) << 4;
                mdb_buf_temp[ptr] += mdb_ascii_to_byte(buf[i + 1]);
                ptr++;
            }
        }
        packet.function     =       mdb_buf_temp[1];
        packet.crc          =  (u16)mdb_buf_temp[ptr - 1];
        if(mdb_function_correct(packet.function) == 0){
            mismatch++;
        }
        if(mdb_lrc8_calc(mdb_buf_temp, ptr - 1) != (u8)packet.crc){
            mismatch++;
        }

        if(mismatch == 0){
            packet.protocol = MDB_PROT_ASCII;
            // Copy temp buffer to input buffer
            memcpy(buf, mdb_buf_temp, ptr);
            packet.slave_addr   =       buf[0];
            packet.reg_addr     = ((u16)buf[2] << 8);
            packet.reg_addr    +=  (u16)buf[3];
            packet.reg_nmb      = ((u16)buf[4] << 8);
            packet.reg_nmb     +=  (u16)buf[5];
            packet.data         =      &buf[6];
        }else{
            // reset packet struct
            memset(&packet, 0, sizeof(mdb_packet_t));
        }
    }

    // Check for ModBUS-RTU
    if(packet.protocol == MDB_PROT_UNKNOWN){
        mismatch = 0;
        packet.slave_addr   =       buf[0];
        packet.function     =       buf[1];
        packet.reg_addr     = ((u16)buf[2] << 8);
        packet.reg_addr    +=  (u16)buf[3];
        packet.reg_nmb      = ((u16)buf[4] << 8);
        packet.reg_nmb     +=  (u16)buf[5];
        packet.data         =      &buf[6];
        packet.crc          = ((u16)buf[len - 1] << 8);
        packet.crc         +=  (u16)buf[len - 2];

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
    u8 lrc8 = 0;

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

    // Make packet for ModBUS-ASCII
    if(packet->protocol == MDB_PROT_ASCII){
        // Fill temp buffer by bytes
        mdb_buf_temp[ptr++] = packet->slave_addr;
        mdb_buf_temp[ptr++] = packet->function;
        memcpy(&mdb_buf_temp[ptr], data, *data_len);
        ptr += *data_len;
        lrc8 = mdb_lrc8_calc(mdb_buf_temp, ptr);
        mdb_buf_temp[ptr++] = lrc8;
        *out_len = ptr;
        ptr = 0;
        // Add start data to out buffer
        out_buf[ptr++] = MDB_ASCII_START;
        // Convert bytes to ASCII in out buffer
        for(u16 i = 0; i < *out_len; i++){
            out_buf[ptr++] = mdb_halfbyte_to_ascii(mdb_buf_temp[i] >> 4);
            out_buf[ptr++] = mdb_halfbyte_to_ascii(mdb_buf_temp[i]);
        }
        out_buf[ptr++] = MDB_ASCII_PREEND;
        out_buf[ptr++] = MDB_ASCII_END;
        *out_len = ptr;
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

/**
 * @brief Calculate LRC8 for ModBUS packet
 * @param buf - pointer to ModBUS packet in bytes (not in ASCII)
 * @param len - packet lenght without LRC
 * @return calculated lrc16 for packet
 */
static u8 mdb_lrc8_calc(u8* buf, u16 len){
    u8 result;

    u16 summ = 0;
    for(u16 i = 0; i < len; i++){
        summ += buf[i];
    }
    summ &= 0xFF;
    result = 0xFF - summ + 1;

    return result;
}

/**
 * @brief Convert ASCII-symboll to byte
 * @param ascii - input ASCII-symbol
 * @ingroup mdb
 * @return byte value of ASCII-symbol
 */
static u8 mdb_ascii_to_byte(u16 ascii){
    u8 result = 0;

    switch(ascii){
    case '0':
        result = 0x00;
        break;
    case '1':
        result = 0x01;
        break;
    case '2':
        result = 0x02;
        break;
    case '3':
        result = 0x03;
        break;
    case '4':
        result = 0x04;
        break;
    case '5':
        result = 0x05;
        break;
    case '6':
        result = 0x06;
        break;
    case '7':
        result = 0x07;
        break;
    case '8':
        result = 0x08;
        break;
    case '9':
        result = 0x09;
        break;
    case 'A':
    case 'a':
        result = 0x0A;
        break;
    case 'B':
    case 'b':
        result = 0x0B;
        break;
    case 'C':
    case 'c':
        result = 0x0C;
        break;
    case 'D':
    case 'd':
        result = 0x0D;
        break;
    case 'E':
    case 'e':
        result = 0x0E;
        break;
    case 'F':
    case 'f':
        result = 0x0F;
        break;

    }

    return result;
}

/**
 * @brief Convert Low half-byte to ASCII-symboll
 * @param byte - input byte
 * @ingroup mdb
 * @return ASCII-symbol of input half-byte
 *
 * @details
 * Input byte masked by 0x0F
 */
static u8 mdb_halfbyte_to_ascii(u8 byte){
    u8 result = 0;

    switch(byte & 0x0F){
    case 0x00:
        result = '0';
        break;
    case 0x01:
        result = '1';
        break;
    case 0x02:
        result = '2';
        break;
    case 0x03:
        result = '3';
        break;
    case 0x04:
        result = '4';
        break;
    case 0x05:
        result = '5';
        break;
    case 0x06:
        result = '6';
        break;
    case 0x07:
        result = '7';
        break;
    case 0x08:
        result = '8';
        break;
    case 0x09:
        result = '9';
        break;
    case 0x0A:
        result = 'A';
        break;
    case 0x0B:
        result = 'B';
        break;
    case 0x0C:
        result = 'C';
        break;
    case 0x0D:
        result = 'D';
        break;
    case 0x0E:
        result = 'E';
        break;
    case 0x0F:
        result = 'F';
        break;
    }

    return result;
}
