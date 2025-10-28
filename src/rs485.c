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

//-------Static functions declaration-----------

static int rs485_gpio_init(void);
static int rs485_gpio_deinit(void);
static int rs485_uart_init(rs485_config_t rs485_config, UART_HandleTypeDef* huart);
static int rs485_uart_deinit(UART_HandleTypeDef* huart);
static int rs485_rcv_timeout_check(rs485_pcb_t* rs485_pcb);

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
    if(rs485_init(rs485_config, &rs485_pcb) == 0){
        service.vars.rs485_state |= SRV_ST_RUN;
        debug_msg(__func__, DBG_MSG_INFO, "RS485_task started");
    }

    uint32_t tick = 0;
    uint32_t last_wake_time = osKernelSysTick();
    while(1){
        if(rs485_pcb.state & RS485_ST_ERROR){
            service.vars.rs485_state |= SRV_ST_ERROR;
            debug_msg(__func__, DBG_MSG_ERR, "RS-485 reinit. Overrun_err = %d, parity_err = %d, frame_err = %d, noise_err = %d",
                      rs485_pcb.err_overrun_cnt, rs485_pcb.err_parity_cnt, rs485_pcb.err_frame_cnt, rs485_pcb.err_noise_cnt);
            rs485_deinit(&rs485_pcb);
            osDelay(1000);
            if(rs485_init(rs485_config, &rs485_pcb) == 0){
                service.vars.rs485_state &= ~(u32)SRV_ST_ERROR;
            }
        }else{
            rs485_rcv_timeout_check(&rs485_pcb);
        }
        if(tick == 5000){
            static char msg[100] = {0};
            sprintf(msg, "Hello world!\n");
            rs485_send(&rs485_pcb, (u8*)msg, strlen(msg));
            tick = 0;
        }

        osDelayUntil(&last_wake_time, RS485_TASK_PERIOD);
        tick++;
    }
}

int rs485_init(rs485_config_t rs485_config, rs485_pcb_t* rs485_pcb){
    int result = 0;

    if(rs485_gpio_init() != 0){
        result = -1;
        debug_msg(__func__, DBG_MSG_ERR, "rs485_gpio_init() error");
    }else{
        rs485_pcb->huart.Instance = _RS485_UART;
        rs485_pcb->buf_in = rs485_buf_in;
        rs485_pcb->buf_rcv = rs485_buf_rcv;
        rs485_pcb->buf_out = rs485_buf_out;

        if(rs485_uart_init(rs485_config, &rs485_pcb->huart) != 0){
            result = -2;
            debug_msg(__func__, DBG_MSG_ERR, "rs485_uart_init() error");
        }else{
        }
    }
    if(result == 0){
        rs485_pcb->state = RS485_ST_READY_RX;

        HAL_NVIC_SetPriority(_RS485_UART_IRQn, RS485_PRIO, RS485_SUBPRIO);
        HAL_NVIC_ClearPendingIRQ(_RS485_UART_IRQn);
        HAL_NVIC_EnableIRQ(_RS485_UART_IRQn);
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
    if(result == 0){
        rs485_pcb->state |= RS485_ST_IN_SENDING;
        HAL_GPIO_WritePin(RS_485_DE_PORT, RS_485_DE_PIN, GPIO_PIN_SET);
        taskENTER_CRITICAL();
        memcpy(rs485_pcb->buf_out, buff, len);
        rs485_pcb->out_ptr = 0;
        rs485_pcb->out_len = len;
        taskEXIT_CRITICAL();
        stat = HAL_UART_Transmit_IT(&rs485_pcb->huart, rs485_pcb->buf_out, rs485_pcb->out_len);
        if(stat != HAL_OK){
            result = -3;
            debug_msg(__func__, DBG_MSG_ERR, "HAL_UART_Transmit_IT() %S", hal_status[stat]);
            rs485_led_err_on_time = 200;
        }else{
            rs485_led_ok_on_time = 200;
        }
    }
    return result;
}

int rs485_irq_callback(rs485_pcb_t* rs485_pcb){
    int result = 0;

    uint32_t data_byte = rs485_pcb->huart.Instance->DR; // Read input data byte
    uint32_t status = rs485_pcb->huart.Instance->SR;    // For interrupt source determination

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

            // Check errors
            if(status & USART_SR_PE){                   // Parity error detected
                rs485_pcb->err_parity_cnt++;
                rs485_pcb->err_total_cnt++;
            }
            if(status & USART_SR_FE){                   // Frame error detected
                rs485_pcb->err_frame_cnt++;
                rs485_pcb->err_total_cnt++;
            }
            if(status & USART_SR_NE){                   // Noise error detected
                rs485_pcb->err_noise_cnt++;
                rs485_pcb->err_total_cnt++;
            }
        }else{
            // Ignore data
        }
    }

    // Transmit data
    if(status & USART_SR_TXE){
        if(rs485_pcb->state & RS485_ST_IN_SENDING){

        }else{
            // Error of interrupt source
            rs485_pcb->err_total_cnt++;
        }
    }


    // transmit mode
    /*if((status & USART_SR_TXE) ){
        if(!(uart_2.state & UART_STATE_IS_LAST_BYTE)){
            if(uart_2.out_ptr>=uart_2.max_len){
                uart_2.out_ptr = uart_2.max_len-1;
            }
            if(uart_2.out_ptr == uart_2.out_len-1){
                huart2.Instance->CR1 &= ~USART_CR1_TXEIE;
                huart2.Instance->CR1 |= USART_CR1_TCIE;
                huart2.Instance->SR &=~USART_SR_TC;
                uart_2.state |= UART_STATE_IS_LAST_BYTE;
                huart2.Instance->DR=uart_2.buff_out[uart_2.out_ptr];
            }else {
                huart2.Instance->DR=uart_2.buff_out[uart_2.out_ptr];
            }
            uart_2.out_ptr++;*/
            /*if(uart_2.out_ptr > uart_2.out_len){
                uart_2.err_cnt++;
            }*/
        /*}else{
            if((status & USART_SR_TC) != USART_SR_TC){
                ; // error
            }
            // end of transmit
            huart2.Instance->CR1 &= ~USART_CR1_TXEIE;
            huart2.Instance->CR1 &= ~USART_CR1_TCIE;
            huart2.Instance->SR &=~USART_SR_TC;
            uart_2.in_ptr = 0;
            uart_2.out_ptr = 0;
            huart2.Instance->CR1 |= USART_CR1_RXNEIE;   // ready to input messages
            huart2.Instance->SR &= ~(USART_SR_RXNE);
            HAL_GPIO_WritePin(RS_485_DE_PORT, RS_485_DE_PIN, GPIO_PIN_RESET);
            uart_2.state |= UART_STATE_SENDED;
            uart_2.state &=~UART_STATE_IS_LAST_BYTE;
            uart_2.state &=~UART_STATE_SENDING;
        }
    }
    // overrun error without RXNE flag
    if(status & USART_SR_ORE){
        uart_2.state |= UART_STATE_ERROR;
        uart_2.overrun_err_cnt++;
        dd=huart2.Instance->SR;
        dd=huart2.Instance->DR;
    }*/
    return result;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    if(huart == &rs485_pcb.huart){
        HAL_GPIO_WritePin(RS_485_DE_PORT, RS_485_DE_PIN, GPIO_PIN_RESET);
        rs485_pcb.state &= ~(u32)RS485_ST_IN_SENDING;
    }
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

    return result;
}

/**
 * @brief Init UART of RS-485 port
 * @param rs485_config - port configuration for init
 * @param huart - pointer to UART_HandleTypeDef of UART
 * @return  0 - ok,\n
 *          -1 - data len error,\n
 *          -2 - stop bit error,\n
 *          -3 - parity error,\n
 *          -4 - HAL_UART_Init() error,\n
 */
static int rs485_uart_init(rs485_config_t rs485_config, UART_HandleTypeDef* huart){
    int result = 0;
    HAL_StatusTypeDef stat = HAL_OK;
    // Enable UART CLK
    _RS485_UART_CLK_EN();

    // Baud rate calculating
    huart->Init.BaudRate = rs485_config.bitrate*100;    // @todo: check baudrate value initialisation
    // Word lenght
    if(result == 0){
        switch (rs485_config.data_size) {
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
            debug_msg(__func__, DBG_MSG_ERR, "UART DATA LEN = %d not supported", rs485_config.data_size);
        }
    }
    // Stop bit
    if(result == 0){
        switch (rs485_config.stop_bit_size) {
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
            debug_msg(__func__, DBG_MSG_ERR, "UART STOP BIT SIZE = %d not supported", rs485_config.stop_bit_size);
        }
    }
    // Parity
    if(result == 0){
        switch (rs485_config.parity) {
        case RS485_PAR_NONE:
            break;
        case RS485_PAR_EVEN:
            break;
        case RS485_PAR_ODD:
            break;
        default:
            result = -3;
            debug_msg(__func__, DBG_MSG_ERR, "UART PARITY code = %d unknown", rs485_config.parity);
        }
    }

    huart->Init.Mode         = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    stat = HAL_UART_Init(huart);
    if (stat != HAL_OK){
        result = -4;
        debug_msg(__func__, DBG_MSG_ERR, "HAL_UART_Init() %S", hal_status[stat]);
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
    // Disable IRQ
    HAL_NVIC_DisableIRQ(_RS485_UART_IRQn);

    return result;
}

/**
 * @brief Check end of packet timeout
 * @param rs485_pcb
 * @return
 */
static int rs485_rcv_timeout_check(rs485_pcb_t* rs485_pcb){
    int result = 0;

    return result;
}

//--------Unrefactoried--------



