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

//--------Macro--------

//--------Typedefs-------


typedef enum{
    MDB_FNCT_RD_MUL_COIL     = 1,    // Read Coil Status
    MDB_FNCT_RD_MUL_DISCR    = 2,    // Read Discrete Inputs
    MDB_FNCT_RD_MUL_HOLD     = 3,    // Read Holding Registers
    MDB_FNCT_RD_MUL_INPUT    = 4,    // Read Input Registers
    MDB_FNCT_WR_SIN_COIL     = 5,    // Force Single Coil
    MDB_FNCT_WR_SIN_HOLD     = 6,    // Preset Single Register
    MDB_FNCT_RD_EXCP_STAT    = 7,    // Read Exception Status
    MDB_FNCT_DIAGNOSTIC      = 8,    // Diagnostic

    MDB_FNCT_RD_EVENT_CNT    = 11,   // Get Com Event Counter
    MDB_FNCT_RD_EVENT_LOG    = 12,   // Get Com Event Log

    MDB_FNCT_WR_MUL_COIL     = 15,   // Force Multiple Coils
    MDB_FNCT_WR_MUL_HOLD     = 16,   // Preset Multiple Registers

    MDB_FNCT_RD_FILE         = 20,   // Read File Record
    MDB_FNCT_WR_FILE         = 21,   // Write File Record
    MDB_FNCT_WR_MASK_REG     = 22,   // Mask Write Register
    MDB_FNCT_RD_WR_MUL_REGS  = 23,   // Read/Write Multiple registers
    MDB_FNCT_RD_FIFO         = 24,   // Read FIFO Queue
}mdb_function_t;

typedef enum{
    MDB_ERR_FUNCT            = 1,    // Unsupported function
    MDB_ERR_ADDR             = 2,    // Register address error
    MDB_ERR_VALUE            = 3,
    MDB_ERR_PARITY           = 8,    // Parity error
}mdb_err_t;

//-------External variables------

//-------Function prototypes----------

/**
 * @brief Recognise packet
 * @param buf - pointer to buffer
 * @param len - lenght of input packet
 * @ingroup mdb
 * @return  0 - packet is not ModBUS,\n
 *          1 - packet is ModBUS-RTU,\n
 *          2 - packet is ModBUS-ASCII
 *          negative value - error,\n *
 */
int mdb_packet_recognise(u8* buf, u16 len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MDB_H
