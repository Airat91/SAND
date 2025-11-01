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

//-------Functions----------

mdb_packet_t mdb_packet_recognise(u8* buf, u16 len){
    mdb_packet_t packet = {0};

    return packet;
}

int mdb_make_response(mdb_packet_t* packet, u8* data, u16 data_len, u8* out_buf){
    int result = 0;

    return result;
}

//-------Static functions----------
