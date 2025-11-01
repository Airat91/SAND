/*
 * File:        mdb.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS functions
 * Revision history: 0.1
 */

#ifndef MDB_H
#define MDB_H
/*add includes below */
#include "type_def.h"
#include "debug.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup mdb
 * @brief Functions for ModBUS
 */

//--------Defines--------

#define MDB_BUF_MAX_LEN     300     // ModBUS packet max lenght
#define MDB_ASCII_START     0x3A    //
#define MDB_FNCT_ERR_FLAG   0x80    // Error flag added into function code

//--------Macro--------

//--------Typedefs-------


typedef enum{
    MDB_FNCT_RD_MUL_COIL    = 1,    // Read Coil Status
    MDB_FNCT_RD_MUL_DISCR   = 2,    // Read Discrete Inputs
    MDB_FNCT_RD_MUL_HOLD    = 3,    // Read Holding Registers
    MDB_FNCT_RD_MUL_INPUT   = 4,    // Read Input Registers
    MDB_FNCT_WR_SIN_COIL    = 5,    // Force Single Coil
    MDB_FNCT_WR_SIN_HOLD    = 6,    // Preset Single Register
    MDB_FNCT_RD_EXCP_STAT   = 7,    // Read Exception Status
    MDB_FNCT_DIAGNOSTIC     = 8,    // Diagnostic

    MDB_FNCT_RD_EVENT_CNT   = 11,   // Get Com Event Counter
    MDB_FNCT_RD_EVENT_LOG   = 12,   // Get Com Event Log

    MDB_FNCT_WR_MUL_COIL    = 15,   // Force Multiple Coils
    MDB_FNCT_WR_MUL_HOLD    = 16,   // Preset Multiple Registers
    MDB_FNCT_RD_SLAVE_ID    = 17,   // Report Slave ID

    MDB_FNCT_RD_FILE        = 20,   // Read File Record
    MDB_FNCT_WR_FILE        = 21,   // Write File Record
    MDB_FNCT_WR_MASK_REG    = 22,   // Mask Write Register
    MDB_FNCT_RD_WR_MUL_REGS = 23,   // Read/Write Multiple registers
    MDB_FNCT_RD_FIFO        = 24,   // Read FIFO Queue
}mdb_function_t;

typedef enum{
    MDB_ERR_FUNCT           = 1,    // Unsupported function
    MDB_ERR_ADDR            = 2,    // Register address error
    MDB_ERR_VALUE           = 3,    // Unavailable value for write
    MDB_ERR_PARITY          = 8,    // Parity error
}mdb_err_t;

typedef enum{
    MDB_PROT_UNKNOWN        = 0,    // ModBUS protocol unknown
    MDB_PROT_RTU            = 1,    // ModBUS-RTU
    MDB_PROT_ASCII          = 2,    // ModBUS-ASCII
    MDB_PROT_TCP            = 3,    // ModBUS-TCP
}mdb_protocol_t;

typedef struct{
    mdb_protocol_t protocol;        // ModBUS protocol type
    u8 slave_addr;                  // Slave address [0..255]
    u8 function;                    // ModBUS function code
    u16 reg_addr;                   // ModBUS register start address
    u16 reg_nmb;                    // Number of registers
    u16 crc;                        // ModBUS packet CRC16
    u8* data;                       // Pointer to data in packet
    u8 data_len;                    // Lenght of data
}mdb_packet_t;

//-------External variables------

//-------Function prototypes----------

/**
 * @brief Recognise packet
 * @param buf - pointer to buffer
 * @param len - lenght of input packet
 * @ingroup mdb
 * @return  mdb_packet_t struct of,\n *
 *
 * If input packet is not ModBUS, mdb_packet_t.protocol = MDB_PROT_UNKNOWN
 */
mdb_packet_t mdb_packet_recognise(u8* buf, u16 len);

/**
 * @brief Make response packet according protocol
 * @param in_packet - pointer to input ModBUS packet
 * @param data - pointer to data buffer for insert to response
 * @param data_len - pointer to data buffer lenght
 * @param out_buf - pointer to place output ModBUS packet
 * @param out_len - pointer to output ModBUS packet lenght
 * @ingroup mdb
 * @return  - 0 - ok,\n
 *          - negative value if error,\n
 *
 * Make ModBUS packet according protocol (RTU or ASCII):
 * 1. Check protocol form in_packet
 * 2. Add function code from in_packet
 * 3. Add data from data buffer
 * 4. Calculate and add CRC
 * 5. Return total out_buf lenght
 */
int mdb_make_response(mdb_packet_t* in_packet, u8* data, u16* data_len, u8* out_buf, u16* out_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MDB_H
