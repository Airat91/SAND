#ifndef PIN_MAP_STM32F103_AI_2_H
#define PIN_MAP_STM32F103_AI_2_H

#if((MCU_DEVICE == STM32F103C8)||(MCU_DEVICE == STM32F103CB))
// For STM device
#if(BOARD_VERSION == 0)
// For PCB_V0
#define AI_MCU_0_PORT       GPIOA
#define AI_MCU_0_PIN        GPIO_PIN_0
#define PWR_CTRL_PORT    	GPIOA
#define PWR_CTRL_PIN     	GPIO_PIN_1
#define RS_485_TX_PORT      GPIOA
#define RS_485_TX_PIN       GPIO_PIN_2
#define RS_485_RX_PORT      GPIOA
#define RS_485_RX_PIN       GPIO_PIN_3
#define RS_485_DE_PORT      GPIOA
#define RS_485_DE_PIN       GPIO_PIN_4
#define AI_MCU_1_PORT       GPIOA
#define AI_MCU_1_PIN        GPIO_PIN_5
#define VREF_PORT           GPIOA
#define VREF_PIN            GPIO_PIN_6
#define MDB_ADDR_7_PORT     GPIOA
#define MDB_ADDR_7_PIN      GPIO_PIN_7
#define MDB_ADDR_2_PORT     GPIOA
#define MDB_ADDR_2_PIN      GPIO_PIN_8
#define MDB_ADDR_1_PORT     GPIOA
#define MDB_ADDR_1_PIN      GPIO_PIN_9
#define MDB_ADDR_0_PORT     GPIOA
#define MDB_ADDR_0_PIN      GPIO_PIN_10
#define USB_N_PORT          GPIOA
#define USB_N_PIN           GPIO_PIN_11
#define USB_P_PORT          GPIOA
#define USB_P_PIN           GPIO_PIN_12
#define SWDIO_PORT          GPIOA
#define SWDIO_PIN           GPIO_PIN_13
#define SWCLK_PORT          GPIOA
#define SWCLK_PIN           GPIO_PIN_14
#define MDB_RATE_0_PORT     GPIOA
#define MDB_RATE_0_PIN      GPIO_PIN_15

#define MDB_ADDR_6_PORT     GPIOB
#define MDB_ADDR_6_PIN      GPIO_PIN_0
#define MDB_ADDR_5_PORT     GPIOB
#define MDB_ADDR_5_PIN      GPIO_PIN_1
#define BOOT_1_PORT         GPIOB
#define BOOT_1_PIN          GPIO_PIN_2
#define MDB_RATE_1_PORT     GPIOB
#define MDB_RATE_1_PIN      GPIO_PIN_3
#define MDB_RATE_2_PORT     GPIOB
#define MDB_RATE_2_PIN      GPIO_PIN_4
#define LED_SYS_R_PORT      GPIOB
#define LED_SYS_R_PIN       GPIO_PIN_5
#define I2C_SCL_PORT        GPIOB
#define I2C_SCL_PIN         GPIO_PIN_6
#define I2C_SDA_PORT        GPIOB
#define I2C_SDA_PIN         GPIO_PIN_7
#define CAN_RX_PORT         GPIOB
#define CAN_RX_PIN          GPIO_PIN_8
#define CAN_TX_PORT         GPIOB
#define CAN_TX_PIN          GPIO_PIN_9
#define MDB_ADDR_4_PORT     GPIOB
#define MDB_ADDR_4_PIN      GPIO_PIN_10
#define MDB_ADDR_3_PORT     GPIOB
#define MDB_ADDR_3_PIN      GPIO_PIN_11
#define MDB_RATE_3_PORT     GPIOB
#define MDB_RATE_3_PIN      GPIO_PIN_12
#define SPI_SCK_PORT        GPIOB
#define SPI_SCK_PIN         GPIO_PIN_13
#define SPI_MISO_PORT       GPIOB
#define SPI_MISO_PIN        GPIO_PIN_14
#define SPI_MOSI_PORT       GPIOB
#define SPI_MOSI_PIN        GPIO_PIN_15

#define LED_SYS_G_PORT      GPIOC
#define LED_SYS_G_PIN       GPIO_PIN_13
#define LED_CON_G_PORT      GPIOC
#define LED_CON_G_PIN       GPIO_PIN_14
#define LED_CON_R_PORT      GPIOC
#define LED_CON_R_PIN       GPIO_PIN_15

#elif(BOARD_VERSION == 1)
// For PCB_V1
#endif // BOARD_VERSION

#elif(MCU_DEVICE == K1986)
// For K1986 device
#if(BOARD_VERSION == 0)
// For PCB_V0
#elif(BOARD_VERSION == 1)
// For PCB_V1
#endif // BOARD_VERSION

#endif // MCU_DEVICE

#endif // PIN_MAP_STM32F103_AI_2_H