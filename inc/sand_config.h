/**
  * The main configuration file. Enable or disable parts of SAND system here.
  *
  * If you add new parts to system, then create #define for control in this file.
  * Each part of system must be represent by pair of source and header files (library).
  * Each name of library function must be beginned by library name.
  * If part is a service, then create service task in main.c/main_task_create().
  *
  */

#ifndef SAND_CONFIG_H
#define SAND_CONFIG_H

#define ADC_EXT_EN      0 // Use external ADC
#define ADC_INT_EN      1 // Use internal ADC
#define CAN_EN          0 // Use CAN interface
#define MODBUS_RTU_EN   1 // Use ModBUS-RTU protocol via UART
#define LWIP_EN         0 // Use LWIP
#define SLIP_EN         0 // Use SLIP protocol via UART
#define CHAIN_EN        0 // Use SPI daisy-chain for expand GPIO
#define RTC_EN          0 // Use RTC
#define TASK_MANAGER_EN 0 // Use task manager for monitor system resources
#define DISPLAY_EN      0 // Use display for HMI
#define BUTTONS_EN      0 // Use buttons for HMI
#define AM2302_EN       0 // Use AM2302 protocol via GPIO
#define DS18B20_EN      0 // Use DS18B20 onewire protocol via GPIO
#define USB_DEVICE_EN   0 // Use USB interface for device mode
#define FLASH_INT_EN    1 // Use internal flash
#define FLASH_EXT_EN    0 // Use external flash
#define LFS_EN          0 // Use LFS file system

#endif // SAND_CONFIG_H
