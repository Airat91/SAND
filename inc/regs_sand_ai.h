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

#define SAND_SAVE_DATA_SIZE      	128

#define OS_STRUCT_SIZE           	92
#define DEVICE_STRUCT_SIZE       	214
#define SERVICE_STRUCT_SIZE      	20
#define TIME_STRUCT_SIZE         	20
#define STORAGE_STRUCT_SIZE      	24
#define MODBUS_STRUCT_SIZE       	10
#define AI_STRUCT_SIZE           	36
#define TEST_STRUCT_SIZE         	870

#define SAND_PROP_BASE_REG_NUM   	73
#define SAND_PROP_MDB_REG_NUM    	73
#define SAND_PROP_RANGE_REG_NUM  	18
#define SAND_PROP_SAVE_REG_NUM   	16
#define SAND_PROP_ACCESS_REG_NUM 	0
//#generator_message{"msg":"sand_struct_define", "action":"insert_end"}

//--------Macro--------

//--------Typedefs-------

//#generator_message{"msg":"sand_struct", "action":"insert_start"}
// This part of file generated automatically, don't change it
typedef union{
	struct MCU_PACK{
		u16    os_version[4];       // "Software version"
		u16    num_of_vars;         // "Total registers in project"
		u16    lang;                // "Interface language"
		u16    command;             // "Command register"
		u16    reset_num;           // "Number of resets"
		u16    reset_reason;        // "Reason of last reset"
		u16    release;             // "Release flag"
		u32    runtime;             // "Runtime from last reset (sec)"
		u32    runtime_total;       // "Total runtime (sec)"
		float  cpu_load;            // "CPU load (%)"
		char   build[40];           // "Build info"
		char   build_date[20];      // "Date of SW building"
	}vars;
	u8 bytes[OS_STRUCT_SIZE];
}os_struct;

typedef union{
	struct MCU_PACK{
		char   device_name[40];     // "Device name"
		char   user_note[40];       // "User device name"
		char   configuration[40];   // "Hardware configuration"
		char   mcu_info[40];        // "MCU information"
		u8     mcu_id[12];          // "Unique MCU ID number"
		u16    device_type;         // "Device type code"
		u16    board_ver;           // "Hardware version"
		u32    serial;              // "Serial number"
		float  temperature;         // "Temperature (°C)"
		float  v_bat;               // "Battery voltage (V)"
		float  v_pwr;               // "Power voltage (V)"
		float  vref_int;            // "Vref internal (V)"
		float  vref_ext;            // "Vref external (V)"
		float  tmpr_add;            // "Temperature calibration additional coeff"
		float  v_pwr_mul;           // "Power voltage calibration multiple coeff"
		float  v_pwr_add;           // "Power voltage calibration additional coe"
		u16    vref_sel;            // "Select Vref source (0-Vref_int, 1-Vref_e"
	}vars;
	u8 bytes[DEVICE_STRUCT_SIZE];
}device_struct;

typedef union{
	struct MCU_PACK{
		u32    rtc_state;           // "RTC service status"
		u32    adc_int_state;       // "ADC_INT service status"
		u32    rs485_state;         // "RS-485 service status"
		u32    dbg_state;           // "Debug service status"
		u32    ai_state;            // "AI service status"
	}vars;
	u8 bytes[SERVICE_STRUCT_SIZE];
}service_struct;

typedef union{
	struct MCU_PACK{
		u16    year;                // "Year"
		u16    month;               // "Month [1..12]"
		u16    day;                 // "Day [1..31]"
		u16    hour;                // "Hour [0..23]"
		u16    minute;              // "Minute [0..59]"
		u16    sec;                 // "Sec [0..59]"
		u16    msec;                // "Msec [0..999]"
		s16    utc;                 // "Utc [-12..14]"
		u32    unix;                // "Unix timestamp"
	}vars;
	u8 bytes[TIME_STRUCT_SIZE];
}time_struct;

typedef union{
	struct MCU_PACK{
		u32    erase_cnt;           // "Storage FLASH erase counter"
		u16    autosave_en;         // "Enable autosave"
		u16    autosave_period;     // "Autosave period (sec)"
		u32    last_save_time;      // "Time of last save"
		u32    dump_addr;           // "Save dump FLASH address"
		u16    dump_position;       // "Save dump FLASH position"
		u16    dump_size;           // "Save dump size (bytes)"
		u16    storage_size;        // "Storage FLASH size (bytes)"
		u16    data_changed;        // "Data changed flag"
	}vars;
	u8 bytes[STORAGE_STRUCT_SIZE];
}storage_struct;

typedef union{
	struct MCU_PACK{
		u16    addr;                // "Device ModBUS address"
		u16    addr_err_ingore;     // "Ignore ILLEGAL DATA ADDRESS error"
		u16    rx_cnt;              // "Received packet counter"
		u16    tx_cnt;              // "Transmitted packet counter"
		u16    err_cnt;             // "Error packet counter"
	}vars;
	u8 bytes[MODBUS_STRUCT_SIZE];
}modbus_struct;

typedef union{
	struct MCU_PACK{
		float  ai_value[2];         // "Measured value"
		float  ai_calib_a[2];       // "Calibration coef A"
		float  ai_calib_b[2];       // "Calibration coef B"
		u16    ai_adc[2];           // "ADC raw data"
		float  ai_sample_rate[2];   // "Samples per second"
	}vars;
	u8 bytes[AI_STRUCT_SIZE];
}ai_struct;

typedef union{
	struct MCU_PACK{
		u8     arr_u8[20];          // "U8 array"
		u16    arr_u16[20];         // "U16 array"
		u32    arr_u32[20];         // "U32 array"
		u64    arr_u64[20];         // "U64 array"
		s8     arr_s8[20];          // "S8 array"
		s16    arr_s16[20];         // "S16 array"
		s32    arr_s32[20];         // "S32 array"
		s64    arr_s64[20];         // "S64 array"
		float  arr_float[20];       // "FLOAT array"
		double arr_double[20];      // "DOUBLE array"
		char   arr_char[30];        // "CHAR array"
	}vars;
	u8 bytes[TEST_STRUCT_SIZE];
}test_struct;

//#generator_message{"msg":"sand_struct", "action":"insert_end"}

//-------External variables------

//#generator_message{"msg":"sand_struct_external", "action":"insert_start"}
// This part of file generated automatically, don't change it
extern os_struct                	os;
extern device_struct            	device;
extern service_struct           	service;
extern time_struct              	time;
extern storage_struct           	storage;
extern modbus_struct            	modbus;
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
