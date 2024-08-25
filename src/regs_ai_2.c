// This file generate automatically. Don't change manually.
#if(MODULE == AI_2)
#include "regs_ai_2.h"

// Global regs structs
regs_info_t regs_info;
regs_system_t regs_system;
regs_rtc_t regs_rtc;

static const u16 def_device_type = MODULE;
static const char def_device_name[] = "SAND-AI-2";
static const u16 def_board_ver = BOARD_VERSION;
static const u16 def_fw_ver[] = {0,0,0};
static const char def_fw_ver_str[] = "0.0.0";
static const char def_fw_build[] = "dev___";

/**
 * @brief List of all self registers in system
 */
//#generator_start {"msg": "regs_c_start", "module": "ai_2", "opt_type": "val_opt_t"}
opt_val_t const regs_list[NUM_OF_SELF_VARS]={
//  p_next_opt  opt_type    p_parent    p_value                         name            description                 type        ind p_default
{{  NULL,       OPT_VAL,    NULL},      (u8*)&regs_info.device_type,    "device_type",  "Device type code",         TYPE_U16,   0,  &def_device_type},
{{  NULL,       OPT_VAL,    NULL},      (u8*)&regs_info.device_name,    "device_name",  "Device name",              TYPE_CHAR,  1,  &def_device_name},
{{  NULL,       OPT_VAL,    NULL},      (u8*)&regs_info.board_ver,      "board_ver",    "Hardware PCB version",     TYPE_U16,   2,  &def_board_ver},
{{  NULL,       OPT_VAL,    NULL},      (u8*)&regs_info.fw_ver,         "fw_ver",       "Firmware version",         TYPE_U16,   3,  &def_fw_ver},
{{  NULL,       OPT_VAL,    NULL},      (u8*)&regs_info.fw_ver_str,     "fw_ver_str",   "Firmware version string",  TYPE_CHAR,  4,  &def_fw_ver_str},
{{  NULL,       OPT_VAL,    NULL},      (u8*)&regs_info.fw_build,       "fw_build",     "Build information",        TYPE_CHAR,  5,  &def_fw_build},
{{  NULL,       OPT_VAL,    NULL},      (u8*)&regs_system.runtime_ms,   "runtime_ms",   "Running time (ms)",        TYPE_U32,   6,  NULL},
{{  NULL,       OPT_VAL,    NULL},      (u8*)&regs_system.runtime_day,  "runtime_day",  "Running time (day)",       TYPE_U16,   7,  NULL},
{{  NULL,       OPT_VAL,    NULL},      (u8*)&regs_system.command,      "command",      "Command register",         TYPE_U16,   8,  NULL},
};
//#generator_start {"msg": "regs_c_end", "module": "ai_2"}

#endif // MDULE
