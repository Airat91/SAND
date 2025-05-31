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
#include "sofi_reg.h"
/*add includes before */
#ifdef __cplusplus
   extern "C" {
#endif

/**
 * @defgroup sand_ai
 * @brief Global structs of SOFI module
 */

//--------Defines--------

//#generator_message{"msg":"sofi_struct_define", "action":"insert_start"}
// This part of file generated automatically, don't change it
#define OS_STRUCT_SIZE           	75
#define TIME_STRUCT_SIZE         	14
#define DEVICE_STRUCT_SIZE       	98
#define MODBUS_STRUCT_SIZE       	1

#define SOFI_PROP_BASE_REG_NUM   	28
#define SOFI_PROP_MDB_REG_NUM    	28
#define SOFI_PROP_RANGE_REG_NUM  	0
#define SOFI_PROP_SAVE_REG_NUM   	9
#define SOFI_PROP_ACCESS_REG_NUM 	1
//#generator_message{"msg":"sofi_struct_define", "action":"insert_end"}

//--------Macro--------

//--------Typedefs-------

//#generator_message{"msg":"sofi_struct", "action":"insert_start"}
// This part of file generated automatically, don't change it
typedef union{
	struct MCU_PACK{
		u8    os_version[4];     // "Software version"
		u16   num_of_vars;       // "Total vars in project"
		u8    lang;              // "Interface language"
		char  build[40];         // "Build info"
		u16   command;           // "Command register"
		u16   reset_num;         // "Number of resets"
		u32   runtime;           // "Runtime from last reset (sec)"
		u32   runtime_total;     // "Total runtime (sec)"
		float cpu_load;          // "CPU load (%)"
		u8    uniq_id[12];       // "Unique ID number"
	}vars;
	u8 bytes[OS_STRUCT_SIZE];
}os_struct;

typedef union{
	struct MCU_PACK{
		u16   year;              // "Year"
		u8    month;             // "Month [1..12]"
		u8    day;               // "Day [1..31]"
		u8    hour;              // "Hour [0..23]"
		u8    minute;            // "Minute [0..59]"
		u8    sec;               // "Sec [0..59]"
		u16   msec;              // "Msec [0..999]"
		u32   unix;              // "Unix timestamp"
		s8    utc;               // "Utc [-12..14]"
	}vars;
	u8 bytes[TIME_STRUCT_SIZE];
}time_struct;

typedef union{
	struct MCU_PACK{
		u8    device_type;       // "Device type code"
		char  device_name[40];   // "Device name"
		u8    board_ver;         // "Hardware version"
		u32   serial;            // "Serial number"
		char  configuration[40]; // "Hardware configuration"
		float temperature;       // "Temperature (°C)"
		float v_bat;             // "Battery voltage (V)"
		float v_pwr;             // "Power voltage (V)"
	}vars;
	u8 bytes[DEVICE_STRUCT_SIZE];
}device_struct;

typedef union{
	struct MCU_PACK{
		u8    addr;              // "Device ModBUS address"
	}vars;
	u8 bytes[MODBUS_STRUCT_SIZE];
}modbus_struct;

//#generator_message{"msg":"sofi_struct", "action":"insert_end"}

//-------External variables------

//#generator_message{"msg":"sofi_struct_external", "action":"insert_start"}
// This part of file generated automatically, don't change it
extern os_struct                	os;
extern time_struct              	time;
extern device_struct            	device;
extern modbus_struct            	modbus;

extern const sofi_prop_base_t   	sofi_prop_base_list[];
extern const sofi_prop_mdb_t    	sofi_prop_mdb_list[];
extern const sofi_prop_range_t  	sofi_prop_range_list[];
extern const sofi_prop_save_t   	sofi_prop_save_list[];
extern const sofi_prop_access_t 	sofi_prop_access_list[];
//#generator_message{"msg":"sofi_struct_external", "action":"insert_end"}

//-------Function prototypes----------

#ifdef __cplusplus
}
#endif
#endif // REGS_SAND_AI_H
