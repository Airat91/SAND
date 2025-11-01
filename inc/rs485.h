/*
 * File:        rs485.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Functions for work with RS-485 based on UART
 * Revision history: 0.1
 */


#ifndef RS485_H
#define RS485_H 1

/*add includes below */
#include "pin_map.h"
#include "cmsis_os.h"
#include "string.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_usart.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "debug.h"
#include "mdb_sand.h"
/*add includes before */

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @defgroup rs485
 * @brief Functions for work with RS-485 based on UART
 */

//--------Defines--------

#define RS485_TASK_PERIOD               2
#define RS485_BAUDRATE_CHECK_PERIOD     1000    // 1 sec
#define RS485_INIT_TIMEOUT_MS           2000    // 2 sec
#define RS485_BUF_LEN                   512     // RS-485 buffers size
#define RS485_CONN_LOST_TIMEOUT         2500    // 2,5 sec
#define RS485_SENDING_TIMEOUT           2000    // 2 sec
#define RS485_RECEIVING_TIMEOUT         2000    // 2 sec
#define RS485_MAX_ERR_NMB               10
#define RS485_LED_BLINK_MS              200     // 200 ms
#define RS485_BTR_VAR_NMB               16      // Number of bitrate variants (see rs485_bitrate_t enum)
#define RS485_DEBUG_PRINT_EN            0       // Print debug data buffer via RS-485
#define RS485_DEBUG_BUF_LEN             RS485_BUF_LEN   //

//========Default config========
#define RS485_DEFAULT_BITRATE           RS485_BTR_115200;
#define RS485_DEFAULT_DATA_LEN          RS485_DATA_8_BIT;
#define RS485_DEFAULT_PARITY            RS485_PAR_NONE;
#define RS485_DEFAULT_STOP_BIT          RS485_STOP_1;
#define RS485_DEFAULT_RX_TIMEOUT_MS     20

//--------Macro--------

#define _RS485_UART             USART2
#define _RS485_UART_CLK_EN      __HAL_RCC_USART2_CLK_ENABLE
#define _RS485_UART_CLK_DIS     __HAL_RCC_USART2_CLK_DISABLE
#define _RS485_UART_IRQn        USART2_IRQn
#define _RS485_DE_EN()          HAL_GPIO_WritePin(RS_485_DE_PORT, RS_485_DE_PIN, GPIO_PIN_SET)
#define _RS485_DE_DIS()         HAL_GPIO_WritePin(RS_485_DE_PORT, RS_485_DE_PIN, GPIO_PIN_RESET)

//========Check GPIO's definitions========
#ifndef RS_485_TX_PORT
    #error "RS-485: Please define RS_485_TX_PORT"
#endif
#ifndef RS_485_TX_PIN
    #error "RS-485: Please define RS_485_TX_PIN"
#endif
#ifndef RS_485_RX_PORT
    #error "RS-485: Please define RS_485_RX_PORT"
#endif
#ifndef RS_485_RX_PIN
    #error "RS-485: Please define RS_485_RX_PIN"
#endif
#ifndef RS_485_DE_PORT
    #error "RS-485: Please define RS_485_DE_PORT"
#endif
#ifndef RS_485_DE_PIN
    #error "RS-485: Please define RS_485_DE_PIN"
#endif
#ifndef RS_485_RATE_0_PIN
    #error "RS-485: Please define RS_485_RATE_0_PIN"
#endif
#ifndef RS_485_RATE_0_PORT
    #error "RS-485: Please define RS_485_RATE_0_PORT"
#endif
#ifndef RS_485_RATE_1_PIN
    #error "RS-485: Please define RS_485_RATE_1_PIN"
#endif
#ifndef RS_485_RATE_1_PORT
    #error "RS-485: Please define RS_485_RATE_1_PORT"
#endif
#ifndef RS_485_RATE_2_PIN
    #error "RS-485: Please define RS_485_RATE_2_PIN"
#endif
#ifndef RS_485_RATE_2_PORT
    #error "RS-485: Please define RS_485_RATE_2_PORT"
#endif
#ifndef RS_485_RATE_3_PIN
    #error "RS-485: Please define RS_485_RATE_3_PIN"
#endif
#ifndef RS_485_RATE_3_PORT
    #error "RS-485: Please define RS_485_RATE_3_PORT"
#endif

//--------Typedefs-------

/**
 * @brief RS-485 state struct
 * @ingroup rs485
 */
typedef enum {
    RS485_ST_DISABLE        = 0,        // Port disabled
    RS485_ST_READY_RX       = (1<<0),   // Port ready for input data
    RS485_ST_ERROR          = (1<<1),   // Error state, reinit need
    RS485_ST_IN_RECEIVE     = (1<<2),   // Receivening process
    RS485_ST_IN_SENDING     = (1<<3),   // Sending process
    RS485_ST_SEND_LAST_BYTE = (1<<4),   // Sending last byte of data
    RS485_ST_WAIT_HANDLING  = (1<<5),   // Set by rx_timeout_ms: buf_in copy to buf_rcv, rcv_len = in_ptr,
                                        // reset in_ptr, set RS485_ST_READY_RX flag and indicate for upper
                                        // layer to parse data in buf_rcv. buf_in ready for new data
    RS485_ST_WAIT_RESPONSE  = (1<<6),   // Port busy for wait response
}rs485_state_t;

typedef enum {                  // Baudrate switches state   1   2   3   4
    RS485_BTR_600    = 6,       // 600 bit/sec              OFF OFF OFF OFF
    RS485_BTR_1200   = 12,      // 1200 bit/sec             OFF OFF OFF ON
    RS485_BTR_2400   = 24,      // 2400 bit/sec             OFF OFF ON  OFF
    RS485_BTR_4800   = 48,      // 4800 bit/sec             OFF OFF ON  ON
    RS485_BTR_9600   = 96,      // 9600 bit/sec             OFF ON  OFF OFF
    RS485_BTR_14400  = 144,     // 14400 bit/sec            OFF ON  OFF ON
    RS485_BTR_19200  = 192,     // 19200 bit/sec            OFF ON  ON  OFF
    RS485_BTR_28800  = 288,     // 28800 bit/sec            OFF ON  ON  ON
    RS485_BTR_38400  = 384,     // 38400 bit/sec            ON  OFF OFF OFF
    RS485_BTR_56000  = 560,     // 56000 bit/sec            ON  OFF OFF ON
    RS485_BTR_57600  = 576,     // 57600 bit/sec            ON  OFF ON  OFF
    RS485_BTR_115200 = 1152,    // 115200 bit/sec           ON  OFF ON  ON
    RS485_BTR_128000 = 1280,    // 128000 bit/sec           ON  ON  OFF OFF
    RS485_BTR_256000 = 2560,    // 256000 bit/sec           ON  ON  OFF ON
    RS485_BTR_512000 = 5120,    // 512000 bit/sec           ON  ON  ON  OFF
    RS485_BTR_1000000 = 10000,  // 1000000 bit/sec          ON  ON  ON  ON
}rs485_bitrate_t;

typedef enum {
    RS485_DATA_8_BIT = 8,       // Data size 8 bits
    RS485_DATA_9_BIT = 9,       // Data size 9 bits
}rs485_data_t;

typedef enum {
    RS485_PAR_NONE   = 0,       // Parity bit not used
    RS485_PAR_EVEN   = 1,       // parity bit is even
    RS485_PAR_ODD    = 2,       // Parity bit is odd
}rs485_parity_t;

typedef enum {
    RS485_STOP_1     = 1,       // Stop bit len is 1 bit
    RS485_STOP_2     = 2,       // Stop bit len is 2 bit
}rs485_stop_t;

/**
 * @brief Struct for UART initialisation
 * @ingroup uart
 */
typedef struct {
    rs485_bitrate_t bitrate;    // Bitrate
    rs485_data_t data_size;     // Data len
    rs485_parity_t parity;      // Parity bit
    rs485_stop_t stop_bit_size; // Stop bit
    u32 rx_timeout_ms;          // End of received packet timeout in ms
}rs485_config_t;

/**
 * @brief UART proccess control block
 * @ingroup uart
 */
typedef struct {
    UART_HandleTypeDef huart;   // Struct fol HAL driver
    rs485_state_t state;        // RS-485 port status flags
    // Buffers
    u8* buf_in;                 // Buffer for save input bytes
    u8* buf_rcv;                // Buffer for operate with received data
    u8* buf_out;                // Buffer for sending data
    u16 in_ptr;                 // Buf_in data pointer
    u16 rcv_ptr;                // Buf_rcv data pointer
    u16 out_ptr;                // Buf_out data pointer
    u16 rcv_len;                // Received data lenght
    u16 out_len;                // Sending data lenght
    // Errors
    u16 err_total_cnt;          // Total errors counter, if > RS485_MAX_ERR_NMB reinit need
    u16 err_overrun_cnt;        // UART overrun errors
    u16 err_parity_cnt;         // UART parity errors
    u16 err_frame_cnt;          // UART frame errors
    u16 err_noise_cnt;          // UART noise errors
    // Timeouts
    u16 timeout_rx_ms_cnt;      // Increase counter in receivening process after last received data
    u16 timeout_resp_ms_cnt;    // Increase counter in response waiting
}rs485_pcb_t;

//-------External variables------

extern rs485_pcb_t rs485_pcb;
extern UART_HandleTypeDef hrs485_uart;
extern osThreadId rs485_task_handle;
extern u32 rs485_led_ok_on_time;
extern u32 rs485_led_err_on_time;

//-------Function prototypes----------

/**
 * @brief Main task of RS-485
 * @param argument - a pointer to arguments
 * @ingroup rs485
 */
void rs485_task(void const * argument);

/**
 * @brief Init RS-485 proccess control block
 * @param rs485_config - pointer to port configuration for init
 * @param rs485_pcb - pointer to RS-485 proccess control block
 * @ingroup rs485
 * @return  0 - ok, \n
 *          -1 - rs485_gpio_init() error, \n
 *          -2 - rs485_uart_init() error, \n
 *
 * 1. Init GPIO
 * 2. Appoint buffers
 * 3. Init UART
 * 4. Enable IRQ
 * 5. Set service_run flag
 */
int rs485_init(rs485_config_t* rs485_config, rs485_pcb_t* rs485_pcb);

/**
 * @brief Deinit RS-485 proccess control block
 * @param rs485_pcb - pointer to RS-485 proccess control block
 * @ingroup rs485
 * @return 0
 *
 * 1. Deinit GPIO
 * 2. Disable IRQ
 * 3. Disable UART_CLK
 * 4. Clear rs485_pcb struct
 */
int rs485_deinit(rs485_pcb_t* rs485_pcb);

/**
 * @brief Send data via RS-485
 * @param rs485_pcb - pointer to RS-485 proccess control block
 * @param buff - pointer to data buffer
 * @param len - lenght of data buffer in bytes
 * @ingroup rs485
 * @return  0 - ok,\n
 *          -1 - out of len,\n
 *          -2 - sending timeout error,\n
 *          -3 - receiving timeout error,\n
 *          -4 - HAL_UART_Transmit_IT() error,\n
 */
int rs485_send(rs485_pcb_t * rs485_pcb, const uint8_t * buff,uint16_t len);

/**
 * @brief Callback function for handle received data
 * @param rs485_pcb - pointer to RS-485 proccess control block
 * @ingroup rs485
 * @return  0 - ok,\n
 *          negative value - error *
 */
int rs485_rx_timeout_callback(rs485_pcb_t* rs485_pcb);

/**
 * @brief Call this function from RS485_UART interrupt
 * @param rs485_pcb - pointer to RS-485 proccess control block
 * @ingroup rs485
 * @return  0 - ok,\n
 *          negative value - error *
 */
int rs485_irq_callback(rs485_pcb_t* rs485_pcb);

#endif // RS485_H
