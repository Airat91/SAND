#ifndef MAIN_CONFIG_H
#define MAIN_CONFIG_H

// Enable/disable services
#define MDB_EN  1
#define CAN_EN  0
#define I2C_EN  0
#define SPI_EN  0
#define AI_EN   0
#define DI_EN   0
#define AO_EN   0
#define DO_EN   0
#define RELE_EN 0
#define USB_EN  0
#define WEB_EN  0
#define RTC_EN  1
#define RS485_EN 1

// IRQ priorities

#define US_TIM_PRIO     10
#define US_TIM_SUBPRIO  0
#define RS485_PRIO     10
#define RS485_SUBPRIO  0

#endif // MAIN_CONFIG_H
