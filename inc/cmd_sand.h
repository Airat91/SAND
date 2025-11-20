/*
 * File:        cmd_sand.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: SAND commands execution
 * Revision history: 0.1
 */
#ifndef CMD_SAND_H
#define CMD_SAND_H 1

/*add includes below */
#include "cmsis_os.h"
#include "debug.h"
/*add includes before */

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @defgroup cmd_sand
 * @brief SAND commands execution
 */

//--------Defines--------

//--------Typedefs-------

typedef enum{
    CMD_NONE                        = 0x0000,   // 0        No command
    // SAND device commands
    CMD_SAND_RESET_DEVICE           = 0x2710,   // 10000    Software reset device
    CMD_SAND_SET_DEFAULT_VALUES     = 0x2711,   // 10001    Set default values

    // SOFI device commands support
    CMD_SOFI_SW_RESET               = 0x5500,   // 21760    Software reset device
    CMD_SOFI_SAVE_ALL_RETAINS       = 0x5501,   // 21761    Save all RETAIN registers
    CMD_SOFI_SET_DEFAULT_VALUES     = 0x5502,   // 21762    Set default values (plc will reboot)
    CMD_SOFI_CHANGE_OS              = 0x5503,   // 21763    Change OS1->OS2 or OS2->OS1
    CMD_SOFI_RUN_OS1                = 0x5504,   // 21764    Run OS1 and wait confirmation
    CMD_SOFI_RUN_OS2                = 0x5505,   // 21765    Run OS2 and wait confirmation
    CMD_SOFI_CONFIRM_OS             = 0x5506,   // 21766    Confirm current OS as main (use after
                                                //          CHANGE_OS, RUN_OS1, RUN_OS2)
    CMD_SOFI_SET_MAIN_AND_RUN_OS1   = 0x5507,   // 21767    Run OS1 without confirmation
    CMD_SOFI_SET_MAIN_AND_RUN_OS2   = 0x5508,   // 21768    Run OS2 without confirmation
    CMD_SOFI_REMOTE_UPDATE_ENABLE   = 0x5509,   // 21769    Ignore BOOTKEY for download OS
    CMD_SOFI_REMOTE_UPDATE_DISABLE  = 0x550A,   // 21770    Check BOOTKEY for downloas OS
    CMD_SOFI_ARC_REMOVE_ALL         = 0x550B,   // 21771    Remove all arc in flash
    CMD_SOFI_LITTLE_FS_FORMAT       = 0x550C,   // 21772    Unmount FS in flash
    CMD_SOFI_SAVE_BKRAM_TO_FLASH    = 0x550D,   // 21773    Save bkram to flash
    CMD_SOFI_SWITCH_TO_BOOT         = 0x550E,   // 21774    Switch to boot mode (for modules only)
    CMD_SOFI_SEARCH_DEVICE_EN       = 0x550F,   // 21775    Special blink system and status leds ON
    CMD_SOFI_SEARCH_DEVICE_DIS      = 0x5510,   // 21776    Special blink system and status leds OFF
    CMD_SOFI_RTC_CALIB_OUT_EN       = 0x5511,   // 21777    Init RTC out 1 Hz on PC13 (SW1)
    CMD_SOFI_RTC_CALIB_OUT_DIS      = 0x5512,   // 21778    Denit RTC out 1 Hz on PC13 (SW1)
    CMD_SOFI_USER_TASK_REMOVE       = 0x5513,   // 21779    Remove USER_TASK from Flash
    CMD_SOFI_ERR_REG_RESET          = 0x5514,   // 21780    Reset err_reg_0 and err_reg_1
}cmd_sand_command_t;

//-------External variables------

//-------Function prototypes----------

/**
 * @brief Execute input command
 * @param command_reg - pointer to command register
 * @ingroup cmd_sand
 * @return 0
 */
int cmd_sand_execute(u16* command_reg);

#ifdef __cplusplus
}
#endif

#endif // CMD_SAND_H
