/*
 * File:        rele.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Work with RELE channels
 * Revision history: 0.1
 */

#ifndef RELE_H
#define RELE_H 1
/*add includes below */
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "type_def.h"
#include "pin_map.h"
#include "reg.h"
#include "sand_reg.h"
#include "debug.h"
/*add includes before */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @defgroup rele
 * @brief Work with RELE channels
 */

//--------Defines--------

#define RELE_MODULE_VER         {0, 1}  // See revision history in file header
#define RELE_CH_NUM             1       // Number of rele channels
#define RELE_TOGGLE_TIMEOUT_MS  100     // Timeout for change RELE state
#define RELE_PERIOD_MS          10      // RELE handle period

//--------Macro--------


//========Check GPIO's definitions========
#ifndef RELE_MCU_1_PORT
    #error "AI: Please define AI_MCU_1_PORT"
#endif
#ifndef RELE_MCU_1_PIN
    #error "AI: Please define AI_MCU_1_PIN"
#endif

//--------Typedefs-------

typedef enum{
    RELE_ST_OFF = 0,                // RELE is OFF
    RELE_ST_ON = 1,                 // RELE is ON
}rele_state_t;

typedef struct{
    rele_state_t state;             // RELE channel state
    rele_state_t last_state;        // Last changed time for timeout check
    u32 last_change;                // Time of last state change
}rele_channel_t;

typedef struct{
    rele_channel_t channel[RELE_CH_NUM];    // RELE channels
}rele_pcb_t;

typedef struct{
    GPIO_TypeDef* port;             // RELE GPIO port
    u16 pin;                        // RELE GPIO pin
}rele_gpio_t;

//-------External variables------

extern rele_pcb_t rele_pcb;         // RELE process control block

//-------Function prototypes----------

/**
 * @brief Init rele process control block
 * @param rele_pcb - pointer to process control block
 * @ingroup rele
 * @return  0 - ok,\n
 *          -1 - rele_gpio_init() error,\n
 *
 * @details
 * 1. Init GPIO's
 * 2. Write rele_version to regs
 * 3. Init rele_pcb
 */
int rele_init(rele_pcb_t* rele_pcb);

/**
 * @brief Denit rele and process control block
 * @param rele_pcb - pointer to process control block
 * @ingroup rele
 * @return  0 - ok,\n
 *          negative value - error
 *
 * @details
 * 1. Deinit GPIO's
 */
int rele_deinit(rele_pcb_t* rele_pcb);

/**
 * @brief Call this function for handle rele operations
 * @param rele_pcb - pointer to process control block
 * @param period_ms - time between function calls
 * @ingroup rele
 * @return  0 - ok,\n
 *          negative value - error
 */
int rele_handle(rele_pcb_t* rele_pcb, u16 period_ms);

/**
 * @brief Update needed vars before reset
 * @ingroup rele
 * @return  0 - ok,\n
 *          negative value - error
 */
int rele_save_before_reset(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // RELE_H
