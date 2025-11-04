/*
 * File:        rs485.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Functions for work with RS-485 based on UART
 * Revision history: 0.1
 */

#include "rs485.h"

//-------Global variables------

rs485_pcb_t rs485_pcb = {0};
osThreadId rs485_task_handle = {0};
u32 rs485_led_ok_on_time = 0;
u32 rs485_led_err_on_time = 0;

//-------Static variables------

static u8 rs485_buf_in[RS485_BUF_LEN]   = {0};
static u8 rs485_buf_rcv[RS485_BUF_LEN]  = {0};
static u8 rs485_buf_out[RS485_BUF_LEN]  = {0};
static const rs485_bitrate_t bitrate_list[RS485_BTR_VAR_NMB] = {
    RS485_BTR_600,
    RS485_BTR_1200,
    RS485_BTR_2400,
    RS485_BTR_4800,
    RS485_BTR_9600,
    RS485_BTR_14400,
    RS485_BTR_19200,
    RS485_BTR_28800,
    RS485_BTR_38400,
    RS485_BTR_56000,
    RS485_BTR_57600,
    RS485_BTR_115200,
    RS485_BTR_128000,
    RS485_BTR_256000,
    RS485_BTR_512000,
    RS485_BTR_1000000,
};

//-------Static functions declaration-----------

static int rs485_gpio_init(void);
static int rs485_gpio_deinit(void);
static int rs485_uart_init(rs485_config_t* rs485_config, UART_HandleTypeDef* huart);
static int rs485_uart_deinit(UART_HandleTypeDef* huart);
static int rs485_rcv_timeout_check(rs485_pcb_t* rs485_pcb, u32 check_time_period);
static int rs485_bitrate_check(rs485_config_t* rs485_config, rs485_pcb_t* rs485_pcb, u32 check_time_period);
static rs485_bitrate_t rs485_read_bitrate_pins(void);
static int rs485_debug_print(void);
static int rs485_state_is_busy(rs485_pcb_t* rs485_pcb);

//-------Functions----------

void rs485_task(void const * argument){
    (void)argument;
    // Init RS-485
    static rs485_config_t rs485_config = {0};
    rs485_config.bitrate        = RS485_DEFAULT_BITRATE;
    rs485_config.data_size      = RS485_DEFAULT_DATA_LEN;
    rs485_config.parity         = RS485_DEFAULT_PARITY;
    rs485_config.stop_bit_size  = RS485_DEFAULT_STOP_BIT;
    rs485_config.rx_timeout_ms  = RS485_DEFAULT_RX_TIMEOUT_MS;
    if(rs485_init(&rs485_config, &rs485_pcb) == 0){
        service.vars.rs485_state |= SRV_ST_RUN;
        debug_msg(__func__, DBG_MSG_INFO, "RS485_task started");
    }

    uint32_t tick = 0;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        if(rs485_pcb.state & RS485_ST_ERROR){
            // Reinit RS-485 if error
            service.vars.rs485_state |= SRV_ST_ERROR;
            debug_msg(__func__, DBG_MSG_ERR, "RS-485 reinit. Overrun_err = %d, parity_err = %d, frame_err = %d, noise_err = %d",
                      rs485_pcb.err_overrun_cnt, rs485_pcb.err_parity_cnt, rs485_pcb.err_frame_cnt, rs485_pcb.err_noise_cnt);
            rs485_deinit(&rs485_pcb);
            osDelay(1000);
            if(rs485_init(&rs485_config, &rs485_pcb) == 0){
                service.vars.rs485_state &= ~(u32)SRV_ST_ERROR;
            }
        }else{
            // Normal work process
            rs485_rcv_timeout_check(&rs485_pcb, RS485_TASK_PERIOD);
            rs485_bitrate_check(&rs485_config, &rs485_pcb, RS485_TASK_PERIOD);
            if(rs485_pcb.state & RS485_ST_WAIT_HANDLING){
                // Send echo
                rs485_pcb.state &= ~(u32)RS485_ST_WAIT_HANDLING;
                rs485_send(&rs485_pcb, rs485_pcb.buf_rcv, rs485_pcb.rcv_len);
            }

#if RS485_DEBUG_PRINT_EN
            rs485_debug_print();
#endif// RS485_DEBUG_PRINT_EN

            // Check error counter
            if(rs485_pcb.err_total_cnt > RS485_MAX_ERR_NMB){
                rs485_pcb.state |= RS485_ST_ERROR;
            }
        }

        osDelayUntil(&last_wake_time, RS485_TASK_PERIOD);
        tick++;
    }
}

int rs485_init(rs485_config_t* rs485_config, rs485_pcb_t* rs485_pcb){
    int result = 0;

    if(rs485_gpio_init() != 0){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "rs485_gpio_init() error");
    }else{
        rs485_pcb->huart.Instance = _RS485_UART;
        rs485_pcb->buf_in = rs485_buf_in;
        rs485_pcb->buf_rcv = rs485_buf_rcv;
        rs485_pcb->buf_out = rs485_buf_out;

        rs485_config->bitrate = rs485_read_bitrate_pins();  // Read actual set bitrate on pins

        if(rs485_uart_init(rs485_config, &rs485_pcb->huart) != 0){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "rs485_uart_init() error");
        }else{
        }
    }
    if(result == 0){
        rs485_pcb->state = RS485_ST_READY_RX;
    }else{
        rs485_pcb->state |= RS485_ST_ERROR;
    }

    return result;
}

int rs485_deinit(rs485_pcb_t* rs485_pcb){
    int result = 0;

    // Deinit GPIO's
    rs485_gpio_deinit();
    // Disable UART
    rs485_uart_deinit(&rs485_pcb->huart);
    // Clear rs485_pcb struct
    memset(rs485_pcb, 0, sizeof(rs485_pcb_t));

    return result;
}

int rs485_send(rs485_pcb_t * rs485_pcb, const uint8_t * buff, uint16_t len){
    int result = 0;
    HAL_StatusTypeDef stat = HAL_OK;

    uint32_t time = us_tim_get_value();
    uint32_t wait = us_tim_get_value() - time;
    if(len > RS485_BUF_LEN){
       result = -1;
    }
    if(result == 0){
        while((rs485_pcb->state & RS485_ST_IN_SENDING)&&(wait < RS485_SENDING_TIMEOUT)){
              wait = (us_tim_get_value() - time);
              osDelay(1);
        }
        if(wait >= RS485_SENDING_TIMEOUT){
            result = -2;
        }
    }
    time = us_tim_get_value();
    wait = us_tim_get_value() - time;
    if(result == 0){
        while((rs485_pcb->state & RS485_ST_IN_RECEIVE)&&(wait < RS485_RECEIVING_TIMEOUT)){
              wait = (us_tim_get_value() - time);
              osDelay(1);
        }
        if(wait >= RS485_RECEIVING_TIMEOUT){
            result = -3;
        }
    }
    if(result == 0){
        rs485_pcb->state |= RS485_ST_IN_SENDING;
        rs485_pcb->state &= ~(u32)RS485_ST_READY_RX;
        _RS485_DE_EN();
        taskENTER_CRITICAL();
        memcpy(rs485_pcb->buf_out, buff, len);
        rs485_pcb->out_ptr = 0;
        rs485_pcb->out_len = len;
        taskEXIT_CRITICAL();
        __HAL_UART_DISABLE_IT(&rs485_pcb->huart, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(&rs485_pcb->huart, UART_IT_TXE);

    }
    if(result < 0){
        rs485_pcb->err_total_cnt++;
        rs485_led_err_on_time = RS485_LED_BLINK_MS;
    }else{
        rs485_pcb->err_total_cnt = 0;
        rs485_led_ok_on_time = RS485_LED_BLINK_MS;
    }

    return result;
}

int rs485_irq_callback(rs485_pcb_t* rs485_pcb){
    int result = 0;
    int error = 0;

    u32 status = rs485_pcb->huart.Instance->SR;    // For interrupt source determination
    u32 data_byte = rs485_pcb->huart.Instance->DR; // Read input data byte

    // Receive data
    if(status & USART_SR_RXNE){
        // Interrupt by received data
        if(rs485_pcb->state & RS485_ST_READY_RX){
            rs485_pcb->timeout_rx_ms_cnt = 0;           // Reset receive timer
            rs485_pcb->state |= RS485_ST_IN_RECEIVE;    // Set receiving proccess flag
            // Add data_byte to buf_in
            if(rs485_pcb->in_ptr < RS485_BUF_LEN){
                rs485_pcb->buf_in[rs485_pcb->in_ptr] = (u8)data_byte;
                rs485_pcb->in_ptr++;
            }else{
                // Input buffer overload
            }

        }else{
            // Ignore data
        }
    }

    // Transmit data
    if(status & USART_SR_TXE){
        // Clear interrupt flag

        if(rs485_pcb->state & RS485_ST_IN_SENDING){
            if((rs485_pcb->state & RS485_ST_SEND_LAST_BYTE) == 0){
                if(rs485_pcb->out_ptr == rs485_pcb->out_len-1){
                    // Last byte sending
                    rs485_pcb->state |= RS485_ST_SEND_LAST_BYTE;
                }
                // Next byte sending
                rs485_pcb->huart.Instance->DR=rs485_pcb->buf_out[rs485_pcb->out_ptr];
                rs485_pcb->out_ptr++;
            }

        }else{
            // Error of interrupt source
            __HAL_UART_DISABLE_IT(&rs485_pcb->huart, UART_IT_TXE);
            // result = -1; // TXE flag is setted ever, so it isn't error
        }
    }

    // Transmit done
    if(status & USART_SR_TC){
        // Clear interrupt flag
        __HAL_UART_CLEAR_FLAG(&rs485_pcb->huart, UART_FLAG_TC);

        if(rs485_pcb->state & RS485_ST_IN_SENDING){
            if(rs485_pcb->state & RS485_ST_SEND_LAST_BYTE){
                // End of transmit
                _RS485_DE_DIS();
                __HAL_UART_DISABLE_IT(&rs485_pcb->huart, UART_IT_TXE);
                __HAL_UART_DISABLE_IT(&rs485_pcb->huart, UART_IT_TC);
                __HAL_UART_ENABLE_IT(&rs485_pcb->huart, UART_IT_RXNE);
                rs485_pcb->state &= ~(u32)RS485_ST_IN_SENDING;
                rs485_pcb->state &= ~(u32)RS485_ST_SEND_LAST_BYTE;
                rs485_pcb->state |= RS485_ST_READY_RX;
            }
        }else{
            // Error of interrupt source
            __HAL_UART_DISABLE_IT(&rs485_pcb->huart, UART_IT_TC);
            result = -1;
        }
    }

    // Check errors
    if(status & USART_SR_PE){                   // Parity error detected
        status &= ~(u32)USART_SR_PE;
        rs485_pcb->err_parity_cnt++;
        result = -2;
    }
    if(status & USART_SR_FE){                   // Frame error detected
        status &= ~(u32)USART_SR_FE;
        rs485_pcb->err_frame_cnt++;
        result = -3;
    }
    if(status & USART_SR_NE){                   // Noise error detected
        status &= ~(u32)USART_SR_NE;
        rs485_pcb->err_noise_cnt++;
        result = -4;
    }
    if(status & USART_SR_ORE){                  // Overrun error detected
        status &= ~(u32)USART_SR_ORE;
        rs485_pcb->err_overrun_cnt++;
        result = -5;
    }
    if(result < 0){
        rs485_pcb->err_total_cnt++;
        rs485_led_err_on_time = RS485_LED_BLINK_MS;
    }else{
        rs485_pcb->err_total_cnt = 0;
    }

    return result;
}

//-------Static functions----------

/**
 * @brief Init UART's GPIOs
 * @ingroup rs485
 * @return 0
 */
static int rs485_gpio_init(void){
    int result = 0;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    GPIO_InitStruct.Pin = RS_485_TX_PIN;
    HAL_GPIO_Init(RS_485_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Pin = RS_485_RX_PIN;
    HAL_GPIO_Init(RS_485_RX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Pin = RS_485_DE_PIN;
    HAL_GPIO_WritePin(RS_485_DE_PORT, RS_485_DE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_Init(RS_485_DE_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Pin = RS_485_RATE_0_PIN;
    HAL_GPIO_Init(RS_485_RATE_0_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = RS_485_RATE_1_PIN;
    HAL_GPIO_Init(RS_485_RATE_1_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = RS_485_RATE_2_PIN;
    HAL_GPIO_Init(RS_485_RATE_2_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = RS_485_RATE_3_PIN;
    HAL_GPIO_Init(RS_485_RATE_3_PORT, &GPIO_InitStruct);

    return result;
}

/**
 * @brief Deinit UART's GPIOs
 * @ingroup rs485
 * @return 0
 */
static int rs485_gpio_deinit(void){
    int result = 0;

    HAL_GPIO_DeInit(RS_485_DE_PORT,RS_485_DE_PIN);
    HAL_GPIO_DeInit(RS_485_RX_PORT,RS_485_RX_PIN);
    HAL_GPIO_DeInit(RS_485_TX_PORT,RS_485_TX_PIN);
    HAL_GPIO_DeInit(RS_485_RATE_0_PORT,RS_485_RATE_0_PIN);
    HAL_GPIO_DeInit(RS_485_RATE_1_PORT,RS_485_RATE_1_PIN);
    HAL_GPIO_DeInit(RS_485_RATE_2_PORT,RS_485_RATE_2_PIN);
    HAL_GPIO_DeInit(RS_485_RATE_3_PORT,RS_485_RATE_3_PIN);

    return result;
}

/**
 * @brief Init UART of RS-485 port
 * @param rs485_config - pointer to port configuration for init
 * @param huart - pointer to UART_HandleTypeDef of UART
 * @return  0 - ok,\n
 *          -1 - data len error,\n
 *          -2 - stop bit error,\n
 *          -3 - parity error,\n
 *          -4 - HAL_UART_Init() error,\n
 */
static int rs485_uart_init(rs485_config_t* rs485_config, UART_HandleTypeDef* huart){
    int result = 0;
    HAL_StatusTypeDef stat = HAL_OK;
    // Enable UART CLK
    _RS485_UART_CLK_EN();

    // Baud rate calculating
    huart->Init.BaudRate = rs485_config->bitrate*100;
    // Word lenght
    if(result == 0){
        switch (rs485_config->data_size) {
        case RS485_DATA_8_BIT:
            huart->Init.WordLength = UART_WORDLENGTH_8B;
            break;
        case RS485_DATA_9_BIT:
            huart->Init.WordLength = UART_WORDLENGTH_9B;
            break;
        default:
            result = -1;
        }
        if(result != 0){
            debug_msg(__func__, DBG_MSG_ERR, "UART DATA LEN = %d not supported", rs485_config->data_size);
        }
    }
    // Stop bit
    if(result == 0){
        switch (rs485_config->stop_bit_size) {
        case RS485_STOP_1:
            huart->Init.StopBits = UART_STOPBITS_1;
            break;
        case RS485_STOP_2:
            huart->Init.StopBits = UART_STOPBITS_2;
            break;
        default:
            result = -2;
        }
        if(result != 0){
            debug_msg(__func__, DBG_MSG_ERR, "UART STOP BIT SIZE = %d not supported", rs485_config->stop_bit_size);
        }
    }
    // Parity
    if(result == 0){
        switch (rs485_config->parity) {
        case RS485_PAR_NONE:
            break;
        case RS485_PAR_EVEN:
            break;
        case RS485_PAR_ODD:
            break;
        default:
            result = -3;
            debug_msg(__func__, DBG_MSG_ERR, "UART PARITY code = %d unknown", rs485_config->parity);
        }
    }

    huart->Init.Mode         = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    stat = HAL_UART_Init(huart);
    if (stat != HAL_OK){
        result = -4;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_UART_Init() %S", hal_status[stat]);
    }else{

        HAL_NVIC_SetPriority(_RS485_UART_IRQn, RS485_PRIO, RS485_SUBPRIO);
        HAL_NVIC_ClearPendingIRQ(_RS485_UART_IRQn);
        HAL_NVIC_EnableIRQ(_RS485_UART_IRQn);

        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_CTS);
        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_LBD);
        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);
        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXNE);
        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(huart, UART_IT_PE);
        __HAL_UART_ENABLE_IT(huart, UART_IT_ERR);
    }

    return result;
}

/**
 * @brief Deinit UART of RS-485 port
 * @param huart - pointer to UART_HandleTypeDef of UART
 * @return  0
 */
static int rs485_uart_deinit(UART_HandleTypeDef* huart){
    int result = 0;

    // Disable UART CLK
    _RS485_UART_CLK_DIS();
    HAL_UART_DeInit(huart);
    // Disable IRQ
    HAL_NVIC_DisableIRQ(_RS485_UART_IRQn);
    __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(huart, UART_IT_PE);
    __HAL_UART_DISABLE_IT(huart, UART_IT_TC);
    __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

    return result;
}

/**
 * @brief Check end of packet timeout
 * @param rs485_pcb - pointer to RS-485 proccess control block
 * @param check_time_period - time period between calls
 * @ingroup rs485
 * @return
 */
static int rs485_rcv_timeout_check(rs485_pcb_t* rs485_pcb, u32 check_time_period){
    int result = 0;

    if(rs485_pcb->state & RS485_ST_IN_RECEIVE){
        rs485_pcb->timeout_rx_ms_cnt += check_time_period;
        if(rs485_pcb->timeout_rx_ms_cnt > RS485_DEFAULT_RX_TIMEOUT_MS){
            // Copy packet from buf_in to buf_rcv and set RS485_ST_WAIT_HANDLING flag
            if(rs485_pcb->state & RS485_ST_WAIT_HANDLING){
                debug_msg(__func__, DBG_MSG_WARN, "Received packet wasn't handle so lost");
                rs485_led_err_on_time = RS485_LED_BLINK_MS;
            }else{
                rs485_led_ok_on_time = RS485_LED_BLINK_MS;
            }
            rs485_pcb->rcv_len = rs485_pcb->in_ptr;
            memcpy(rs485_pcb->buf_rcv, rs485_pcb->buf_in, rs485_pcb->rcv_len);
            rs485_pcb->in_ptr = 0;
            rs485_pcb->state |= RS485_ST_WAIT_HANDLING;
            rs485_pcb->state &= ~(u32)RS485_ST_IN_RECEIVE;

            // Check for ModBUS
            mdb_packet_t mdb_in_packet = mdb_packet_recognise(rs485_pcb->buf_rcv, rs485_pcb->rcv_len);
            if(mdb_in_packet.protocol != MDB_PROT_UNKNOWN){
                if(mdb_sand_packet_handle(&mdb_sand_pcb, &mdb_in_packet) == 0){
                    rs485_pcb->state &= ~(u32)RS485_ST_WAIT_HANDLING;
                    if(mdb_sand_pcb.resp_len > 0){
                        rs485_send(rs485_pcb, mdb_sand_pcb.resp_buf, mdb_sand_pcb.resp_len);
                    }
                }else{
                    // mdb_sand_packet_handle() error
                }
            }
        }
    }

    if(rs485_pcb->state & RS485_ST_WAIT_RESPONSE){
        rs485_pcb->timeout_resp_ms_cnt += check_time_period;
        if(rs485_pcb->timeout_resp_ms_cnt > RS485_CONN_LOST_TIMEOUT){
            // Response time is out
        }
    }

    return result;
}

/**
 * @brief Check bitate configuration pins and reinit if changed
 * @param rs485_config - pointer to port configuration for init
 * @param rs485_pcb - pointer to RS-485 proccess control block
 * @param check_time_period - time period between calls
 * @ingroup rs485
 * @return  0 - no changed,\n
 *          1 - baudrate was changed,\n
 *          negative value - error,\n
 */
static int rs485_bitrate_check(rs485_config_t* rs485_config, rs485_pcb_t* rs485_pcb, u32 check_time_period){
    int result = 0;
    static u32 tick = 0;
    u8 bitrate_pins = 0;
    rs485_bitrate_t bitrate_current = rs485_config->bitrate;
    rs485_bitrate_t bitrate_new = rs485_read_bitrate_pins();

    if(tick > RS485_BAUDRATE_CHECK_PERIOD){
        if(bitrate_new == bitrate_current){
            // Reset tick counter
            tick = 0;
        }else{
            // Reinit uart with new baudrate
            rs485_config->bitrate = bitrate_new;
            rs485_uart_deinit(&rs485_pcb->huart);
            rs485_pcb->state = RS485_ST_DISABLE;
            if(rs485_uart_init(rs485_config, &rs485_pcb->huart) != 0){
                result = -1;
                debug_msg(__func__, DBG_MSG_ERR, "rs485_uart_init() error");
            }else{
                rs485_pcb->state = RS485_ST_READY_RX;
                debug_msg(__func__, DBG_MSG_INFO, "Bitrate changed to %d bit/s", bitrate_new * 100);
            }
            tick = 0;
        }
    }else{
        tick += check_time_period;
    }

    return result;
}

/**
 * @brief Read baudrate switches state
 * @ingroup rs485
 * @return rs485_bitrate_t enum value
 */
static rs485_bitrate_t rs485_read_bitrate_pins(void){
    rs485_bitrate_t result = 0;

    u8 bitrate_pins = 0;
    if(HAL_GPIO_ReadPin(RS_485_RATE_3_PORT, RS_485_RATE_3_PIN)){
        bitrate_pins |= 0x01;
    }
    if(HAL_GPIO_ReadPin(RS_485_RATE_2_PORT, RS_485_RATE_2_PIN)){
        bitrate_pins |= 0x02;
    }
    if(HAL_GPIO_ReadPin(RS_485_RATE_1_PORT, RS_485_RATE_1_PIN)){
        bitrate_pins |= 0x04;
    }
    if(HAL_GPIO_ReadPin(RS_485_RATE_0_PORT, RS_485_RATE_0_PIN)){
        bitrate_pins |= 0x08;
    }
    result = bitrate_list[bitrate_pins];

    return result;
}

/**
 * @brief Print debug buffer via RS-485 interface
 * @ingroup rs485
 * @return 0
 *
 * 1. Read debug buffer and check its lenght
 * 2. Check RS-485 interface state
 * 3. Send data to RS-485 interface
 * 4. If debug buffer data larger than RS-485 out buffer, send only RS-485 out len max
 */
static int rs485_debug_print(){
    int result = 0;
#if RS485_DEBUG_PRINT_EN
    static char temp_buf[RS485_DEBUG_BUF_LEN] = {0};

    // Read debug buffer data lenght
     u16 data_len = debug_buf_get_len();
     if(data_len > 0){
         // Check out of RS-485 max len
         if(data_len > RS485_DEBUG_BUF_LEN){
             data_len = RS485_DEBUG_BUF_LEN;
         }
         // Check RS-485 state
         if(rs485_state_is_busy(&rs485_pcb) == 0){
             debug_buf_read(temp_buf, data_len, DEBUG_BUF_READ_TIMEOUT_US);
             rs485_send(&rs485_pcb, (u8*)temp_buf, data_len);
         }else{
             result = -1;
         }
     }

#endif // RS485_DEBUG_PRINT_EN
    return result;
}

/**
 * @brief Read rs485_pcb state
 * @param rs485_pcb - pointer to RS-485 proccess control block
 * @ingroup rs485
 * @return  0 - rs485_pcb not busy,\n
 *          positive value - rs485_state_t flags,\n
 */
static int rs485_state_is_busy(rs485_pcb_t* rs485_pcb){
    int result = 0;

    result |= (rs485_pcb->state & RS485_ST_IN_RECEIVE);
    result |= (rs485_pcb->state & RS485_ST_IN_SENDING);
    result |= (rs485_pcb->state & RS485_ST_WAIT_RESPONSE);
    result |= (rs485_pcb->state & RS485_ST_ERROR);

    return result;
}
