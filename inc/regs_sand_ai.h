/*
 * File:        regs_sand_ai.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global structs of SOFI module
 * Revision history: 0.1
 */

#ifndef REGS_SAND_AI_H
#define REGS_SAND_AI_H 1

/*add includes below */
#include "type_def.h"
#include "sand_reg.h"
/*add includes before */
#ifdef __cplusplus
   extern "C" {
#endif

/**
 * @defgroup sand_ai
 * @brief Global structs of SOFI module
 */

//--------Defines--------

//#generator_message{"msg":"sand_struct_define", "action":"insert_start"}
// This part of file generated automatically, don't change it
#define DEVICE_NAME              	"SAND_AI"

#define OS_STRUCT_SIZE           	88
#define TIME_STRUCT_SIZE         	20
#define DEVICE_STRUCT_SIZE       	200
#define MODBUS_STRUCT_SIZE       	2
#define SERVICE_STRUCT_SIZE      	20
#define AI_STRUCT_SIZE           	36
#define TEST_STRUCT_SIZE         	860

#define SAND_PROP_BASE_REG_NUM   	54
#define SAND_PROP_MDB_REG_NUM    	54
#define SAND_PROP_RANGE_REG_NUM  	9
#define SAND_PROP_SAVE_REG_NUM   	10
#define SAND_PROP_ACCESS_REG_NUM 	0
//#generator_message{"msg":"sand_struct_define", "action":"insert_end"}

//--------Macro--------

//--------Typedefs-------

//#generator_message{"msg":"sand_struct", "action":"insert_start"}
// This part of file generated automatically, don't change it
typedef union{
	struct MCU_PACK{
		u16    os_version[4];      // "Software version"
		u16    num_of_vars;        // "Total registers in project"
		u16    lang;               // "Interface language"
		char   build[40];          // "Build info"
		char   build_date[20];     // "Date of SW building"
		u16    command;            // "Command register"
		u16    reset_num;          // "Number of resets"
		u32    runtime;            // "Runtime from last reset (sec)"
		u32    runtime_total;      // "Total runtime (sec)"
		float  cpu_load;           // "CPU load (%)"
	}vars;
	u8 bytes[OS_STRUCT_SIZE];
}os_struct;

typedef union{
	struct MCU_PACK{
		u16    year;               // "Year"
		u16    month;              // "Month [1..12]"
		u16    day;                // "Day [1..31]"
		u16    hour;               // "Hour [0..23]"
		u16    minute;             // "Minute [0..59]"
		u16    sec;                // "Sec [0..59]"
		u16    msec;               // "Msec [0..999]"
		u32    unix;               // "Unix timestamp"
		s16    utc;                // "Utc [-12..14]"
	}vars;
	u8 bytes[TIME_STRUCT_SIZE];
}time_struct;

typedef union{
	struct MCU_PACK{
		char   device_name[40];    // "Device name"
		char   user_name[40];      // "User device name"
		char   configuration[40];  // "Hardware configuration"
		char   mcu_info[40];       // "MCU information"
		u8     mcu_id[12];         // "Unique MCU ID number"
		u16    device_type;        // "Device type code"
		u16    board_ver;          // "Hardware version"
		u32    serial;             // "Serial number"
		float  temperature;        // "Temperature (°C)"
		float  v_bat;              // "Battery voltage (V)"
		float  v_pwr;              // "Power voltage (V)"
		float  vref_int;           // "Vref internal (V)"
		float  vref_ext;           // "Vref external (V)"
	}vars;
	u8 bytes[DEVICE_STRUCT_SIZE];
}device_struct;

typedef union{
	struct MCU_PACK{
		u16    addr;               // "Device ModBUS address"
	}vars;
	u8 bytes[MODBUS_STRUCT_SIZE];
}modbus_struct;

typedef union{
	struct MCU_PACK{
		u32    rtc_state;          // "RTC service status"
		u32    adc_int_state;      // "ADC_INT service status"
		u32    rs485_state;        // "RS-485 service status"
		u32    dbg_state;          // "Debug service status"
		u32    ai_state;           // "AI service status"
	}vars;
	u8 bytes[SERVICE_STRUCT_SIZE];
}service_struct;

typedef union{
	struct MCU_PACK{
		float  ai_value[2];        // "Measured value"
		float  ai_calib_a[2];      // "Calibration coef A"
		float  ai_calib_b[2];      // "Calibration coef B"
		u16    ai_adc[2];          // "ADC raw data"
		float  ai_sample_rate[2];  // "Samples per second"
	}vars;
	u8 bytes[AI_STRUCT_SIZE];
}ai_struct;

typedef union{
	struct MCU_PACK{
		u8     arr_u8[20];         // "None"
		u16    arr_u16[20];        // "None"
		u32    arr_u32[20];        // "None"
		u64    arr_u64[20];        // "None"
		s8     arr_s8[20];         // "None"
		s16    arr_s16[20];        // "None"
		s32    arr_s32[20];        // "None"
		s64    arr_s64[20];        // "None"
		float  arr_float[20];      // "None"
		double arr_double[20];     // "None"
		char   arr_char[20];       // "None"
	}vars;
	u8 bytes[TEST_STRUCT_SIZE];
}test_struct;

//#generator_message{"msg":"sand_struct", "action":"insert_end"}

//-------External variables------

//#generator_message{"msg":"sand_struct_external", "action":"insert_start"}
// This part of file generated automatically, don't change it
extern os_struct                	os;
extern time_struct              	time;
extern device_struct            	device;
extern modbus_struct            	modbus;
extern service_struct           	service;
extern ai_struct                	ai;
extern test_struct              	test;

extern const sand_prop_base_t   	sand_prop_base_list[];
extern const sand_prop_mdb_t    	sand_prop_mdb_list[];
extern const sand_prop_range_t  	sand_prop_range_list[];
extern const sand_prop_save_t   	sand_prop_save_list[];
extern const sand_prop_access_t 	sand_prop_access_list[];
//#generator_message{"msg":"sand_struct_external", "action":"insert_end"}

//-------Function prototypes----------

#ifdef __cplusplus
}
#endif

#endif // REGS_SAND_AI_H
