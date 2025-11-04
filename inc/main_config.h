#ifndef MAIN_CONFIG_H
#define MAIN_CONFIG_H

// Enable/disable services
#define MDB_EN      1
#define CAN_EN      0
#define I2C_EN      0
#define SPI_EN      0
#define USB_EN      0
#define WEB_EN      0
#define RTC_EN      1
#define RS485_EN    1

#if(DEVICE_TYPE == SAND_AI)
    #define AI_EN       1
#elif(DEVICE_TYPE == SAND_DI)
    #define DI_EN       1
#elif(DEVICE_TYPE == SAND_AO)
    #define AO_EN       1
#elif(DEVICE_TYPE == SAND_D))
    #define DO_EN       1
#elif(DEVICE_TYPE == SAND_RELE)
    #define RELE_EN     1
#endif // DEVICE_TYPE

// IRQ priorities

#define US_TIM_PRIO     10
#define US_TIM_SUBPRIO  0
#define RS485_PRIO      10
#define RS485_SUBPRIO   0
#define ADC_INT_PRIO    10
#define ADC_INT_SUBPRIO 0
#define AI_PRIO         10
#define AI_SUBPRIO      0

#endif // MAIN_CONFIG_H
