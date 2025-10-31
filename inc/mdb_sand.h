/*
 * File:        mdb_sand.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: ModBUS API functions for SAND integration
 * Revision history: 0.1
 */

#ifndef MDB_SAND_H
#define MDB_SAND_H
/*add includes below */
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "type_def.h"
#include "mdb.h"
#include "pin_map.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//--------Defines--------

#define MDB_TASK_PERIOD      10
#define MDB_INIT_TIMEOUT_MS  2000
#define MDB_SUPRT_FNCT_NMB   4

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
    MDB_ST_DISABLE   = 0,
    MDB_ST_READY     = (1<<0),
}mdb_sand_state_t;

typedef struct{
    mdb_sand_state_t state;
    u8 out_buf[MDB_BUF_MAX_LEN];
    u16 out_len;
}mdb_sand_pcb_t;

typedef struct{
    u8 slave_addr;  // Slave address [0..255]
    u8 function;    // ModBUS function code
    u16 reg_addr;   // ModBUS register start address
    u16 reg_nmb;    // Number of registers
    u16 crc;        // ModBUS packet CRC16
    u16* data;      // Pointer to data in packet
    u8 data_len;    // Lenght of data
}mdb_packet_t;

//-------External variables------

extern osThreadId mdb_task_handle;
extern mdb_sand_pcb_t mdb_sand_pcb;

//-------Function prototypes----------

/**
 * @brief Main task of ModBUS service
 * @param argument - a pointer to arguments
 * @ingroup modbus
 */
void mdb_sand_task(void const * argument);

/**
 * @brief Init ModBUS process control block
 * @ingroup modbus
 * @param mdb_sand_pcb - pointer to ModBUS proccess control block
 * @return  0 - ok,\n
 *          negative value if error
 */
int mdb_sand_init(mdb_sand_pcb_t* mdb_sand_pcb);

/**
 * @brief Deinit ModBUS process control block
 * @ingroup modbus
 * @param mdb_sand_pcb - pointer to ModBUS proccess control block
 * @return  0 - ok,\n
 *          negative value if error
 */
int mdb_sand_deinit(mdb_sand_pcb_t* mdb_sand_pcb);

/**
 * @brief Read ModBUs address switches state
 * @ingroup modbus
 * @return self modbus address
 */
int mdb_sand_read_addr(void);

/**
 * @brief Handle input ModBUS packet
 * @param mdb_sand_pcb - pointer to ModBUS proccess control block
 * @param buf - pointer to packet buffer
 * @param len - lenght of buffer
 * @return  0 - packet handled, no need response,\n
 *          1 - packet handled, send response from mdb_pcb.out_buf
 *          -1 - unavailable registers address
 */
int mdb_packet_handle(mdb_sand_pcb_t* mdb_sand_pcb, u8* buf, u16 len);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MDB_SAND_H
