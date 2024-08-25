#ifndef REGS_AI_2_H
#define REGS_AI_2_H
// This file generate automatically. Don't change manually.
#include "sand_config.h"
#include "regs.h"

//#generator_start {"msg": "define_start", "module": "ai_2"}
#define NUM_OF_SELF_VARS 50
#define AI_NUM 2
//#generator_end {"msg": "define_end"}


/**
 * @brief regs_info
 * @ingroup regs
 */
//#generator_start {"msg": "regs_h_start", "module": "ai_2", "stuct_name" :"regs_info"}
typedef struct MCU_PACK{
    u16 device_type;
    u8 device_name[40];
    u16 board_ver;
    u16 fw_ver[3];
    u8 fw_ver_str[40];
    u8 fw_build[40];

}regs_info_t;
extern regs_info_t regs_info;
//#generator_end {"msg": "regs_h_end"}

/**
 * @brief regs_system
 * @ingroup regs
 */
//#generator_start {"msg": "regs_h_start", "module": "ai_2", "stuct_name" :"regs_system"}
typedef struct MCU_PACK{
    u32 runtime_ms;
    u16 runtime_day;
    u16 command;

}regs_system_t;
extern regs_system_t regs_system;
//#generator_end {"msg": "regs_h_end"}

/**
 * @brief regs_rtc
 * @ingroup regs
 */
//#generator_start {"msg": "regs_h_start", "module": "ai_2", "stuct_name" :"regs_rtc"}
typedef struct MCU_PACK{
    u8 hour;
    u8 min;
    u8 sec;
    u16 msec;
    u8 weekday;
    u8 day;
    u8 month;
    u16 year;
    u32 unix_time;
    s8 utc;
}regs_rtc_t;
extern regs_rtc_t regs_rtc;
//#generator_end {"msg": "regs_h_end"}

#endif // REGS_AI_2_H
