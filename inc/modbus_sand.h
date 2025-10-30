/*
 * File:        modbus_sand.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS API functions for SAND integration
 * Revision history: 0.1
 */

#ifndef MODBUS_SAND_H
#define MODBUS_SAND_H
/*add includes below */
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "type_def.h"
#include "pin_map.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup modbus
 * @brief Functions for ModBUS
 */

//--------Defines--------

#define MODBUS_TASK_PERIOD      10
#define MODBUS_INIT_TIMEOUT_MS  2000
#define MODBUS_BUF_MAX_LEN      300
#define MODBUS_SUPRT_FNCT_NMB   4

//--------Macro--------

//========Check GPIO's definitions========
#ifndef MDB_ADDR_0_PORT
    #error "MODBUS: Please define MDB_ADDR_0_PORT"
#endif
#ifndef MDB_ADDR_0_PIN
    #error "MODBUS: Please define MDB_ADDR_0_PIN"
#endif
#ifndef MDB_ADDR_1_PORT
    #error "MODBUS: Please define MDB_ADDR_1_PORT"
#endif
#ifndef MDB_ADDR_1_PIN
    #error "MODBUS: Please define MDB_ADDR_1_PIN"
#endif
#ifndef MDB_ADDR_2_PORT
    #error "MODBUS: Please define MDB_ADDR_2_PORT"
#endif
#ifndef MDB_ADDR_2_PIN
    #error "MODBUS: Please define MDB_ADDR_2_PIN"
#endif
#ifndef MDB_ADDR_3_PORT
    #error "MODBUS: Please define MDB_ADDR_3_PORT"
#endif
#ifndef MDB_ADDR_3_PIN
    #error "MODBUS: Please define MDB_ADDR_3_PIN"
#endif
#ifndef MDB_ADDR_4_PORT
    #error "MODBUS: Please define MDB_ADDR_4_PORT"
#endif
#ifndef MDB_ADDR_4_PIN
    #error "MODBUS: Please define MDB_ADDR_4_PIN"
#endif
#ifndef MDB_ADDR_5_PORT
    #error "MODBUS: Please define MDB_ADDR_5_PORT"
#endif
#ifndef MDB_ADDR_5_PIN
    #error "MODBUS: Please define MDB_ADDR_5_PIN"
#endif
#ifndef MDB_ADDR_6_PORT
    #error "MODBUS: Please define MDB_ADDR_6_PORT"
#endif
#ifndef MDB_ADDR_6_PIN
    #error "MODBUS: Please define MDB_ADDR_6_PIN"
#endif
#ifndef MDB_ADDR_7_PORT
    #error "MODBUS: Please define MDB_ADDR_7_PORT"
#endif
#ifndef MDB_ADDR_7_PIN
    #error "MODBUS: Please define MDB_ADDR_7_PIN"
#endif

//--------Typedefs-------

typedef enum{
    MODBUS_ST_DISABLE   = 0,
    MODBUS_ST_READY     = (1<<0),
}modbus_state_t;

typedef enum{
    MODBUS_FNCT_RD_MUL_COIL     = 1,    // Read Coil Status
    MODBUS_FNCT_RD_MUL_DISCR    = 2,    // Read Discrete Inputs
    MODBUS_FNCT_RD_MUL_HOLD     = 3,    // Read Holding Registers
    MODBUS_FNCT_RD_MUL_INPUT    = 4,    // Read Input Registers
    MODBUS_FNCT_WR_SIN_COIL     = 5,    // Force Single Coil
    MODBUS_FNCT_WR_SIN_HOLD     = 6,    // Preset Single Register
    MODBUS_FNCT_RD_EXCP_STAT    = 7,    // Read Exception Status
    MODBUS_FNCT_DIAGNOSTIC      = 8,    // Diagnostic

    MODBUS_FNCT_RD_EVENT_CNT    = 11,   // Get Com Event Counter
    MODBUS_FNCT_RD_EVENT_LOG    = 12,   // Get Com Event Log

    MODBUS_FNCT_WR_MUL_COIL     = 15,   // Force Multiple Coils
    MODBUS_FNCT_WR_MUL_HOLD     = 16,   // Preset Multiple Registers

    MODBUS_FNCT_RD_FILE         = 20,   // Read File Record
    MODBUS_FNCT_WR_FILE         = 21,   // Write File Record
    MODBUS_FNCT_WR_MASK_REG     = 22,   // Mask Write Register
    MODBUS_FNCT_RD_WR_MUL_REGS  = 23,   // Read/Write Multiple registers
    MODBUS_FNCT_RD_FIFO         = 24,   // Read FIFO Queue
}modbus_function_t;

typedef enum{
    MODBUS_ERR_FUNCT            = 1,    // Unsupported function
    MODBUS_ERR_ADDR             = 2,    // Register address error
    MODBUS_ERR_VALUE            = 3,
    MODBUS_ERR_PARITY           = 8,    // Parity error
}modbus_err_t;

typedef struct{
    modbus_state_t state;
    u8 out_buf[MODBUS_BUF_MAX_LEN];
    u16 out_len;
}modbus_pcb_t;

typedef struct{
    u8 slave_addr;  // Slave address [0..255]
    u8 function;    // ModBUS function code
    u16 reg_addr;   // ModBUS register start address
    u16 reg_nmb;    // Number of registers
    u16 crc;        // ModBUS packet CRC16
    u16* data;      // Pointer to data in packet
    u8 data_len;    // Lenght of data
}modbus_packet_t;

//-------External variables------

extern osThreadId modbus_task_handle;
extern modbus_pcb_t modbus_pcb;

//-------Function prototypes----------

/**
 * @brief Main task of ModBUS service
 * @param argument - a pointer to arguments
 * @ingroup modbus
 */
void modbus_task(void const * argument);

/**
 * @brief Init ModBUS process control block
 * @ingroup modbus
 * @param modbus_pcb - pointer to ModBUS proccess control block
 * @return  0 - ok,\n
 *          negative value if error
 */
int modbus_init(modbus_pcb_t* modbus_pcb);

/**
 * @brief Deinit ModBUS process control block
 * @ingroup modbus
 * @param modbus_pcb - pointer to ModBUS proccess control block
 * @return  0 - ok,\n
 *          negative value if error
 */
int modbus_deinit(modbus_pcb_t* modbus_pcb);

/**
 * @brief Read ModBUs address switches state
 * @ingroup modbus
 * @return self modbus address
 */
int modbus_read_addr(void);

/**
 * @brief Recognise packet as ModBUS
 * @param buf - pointer to packet buffer
 * @param len - lenght of buffer
 * @ingroup modbus
 * @return  0 - packet isn't ModBUS
 *          1 - packet is ModBUS
 */
int modbus_packet_recognise(u8* buf, u16 len);

/**
 * @brief Handle input ModBUS packet
 * @param modbus_pcb - pointer to ModBUS proccess control block
 * @param buf - pointer to packet buffer
 * @param len - lenght of buffer
 * @return  0 - packet handled, no need response,\n
 *          1 - packet handled, send response from modbus_pcb.out_buf
 *          -1 - ModBUS function unsupported
 *          -2 - unavailable registers address
 */
int modbus_packet_handle(modbus_pcb_t* modbus_pcb, u8* buf, u16 len);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MODBUS_SAND_H
