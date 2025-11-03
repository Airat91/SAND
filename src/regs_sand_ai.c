/*
 * File:        regs_sand_ai.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global structs of SOFI module
 * Revision history: 0.1
 */

#include "regs_sand_ai.h"

//#generator_message{"msg":"sofi_properties", "action":"insert_start"}
// This part of file generated automatically, don't change it
os_struct      	os;
time_struct    	time;
device_struct  	device;
modbus_struct  	modbus;
service_struct 	service;
test_struct    	test;

const sofi_prop_base_t sofi_prop_base_list[SOFI_PROP_BASE_REG_NUM]={
//prop           	header_next                    	header_base                     	prop_num 	p_value                         	name            	description                     	type            	array_len 	ind 	read_only 	
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[0], 	(void*)&sofi_prop_base_list[0]}, 	2,       	(u8*)&os.vars.os_version,       	"os_version",   	"Software version",             	VAR_TYPE_U16,   	4,        	0,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[1], 	(void*)&sofi_prop_base_list[1]}, 	2,       	(u8*)&os.vars.num_of_vars,      	"num_of_vars",  	"Total vars in project",        	VAR_TYPE_U16,   	1,        	1,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[2], 	(void*)&sofi_prop_base_list[2]}, 	3,       	(u8*)&os.vars.lang,             	"lang",         	"Interface language",           	VAR_TYPE_U16,   	1,        	2,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[3], 	(void*)&sofi_prop_base_list[3]}, 	2,       	(u8*)&os.vars.build,            	"build",        	"Build info",                   	VAR_TYPE_CHAR,  	40,       	3,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[4], 	(void*)&sofi_prop_base_list[4]}, 	2,       	(u8*)&os.vars.build_date,       	"build_date",   	"Date of SW building",          	VAR_TYPE_CHAR,  	40,       	4,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[5], 	(void*)&sofi_prop_base_list[5]}, 	2,       	(u8*)&os.vars.command,          	"command",      	"Command register",             	VAR_TYPE_U16,   	1,        	5,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[6], 	(void*)&sofi_prop_base_list[6]}, 	3,       	(u8*)&os.vars.reset_num,        	"reset_num",    	"Number of resets",             	VAR_TYPE_U16,   	1,        	6,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[7], 	(void*)&sofi_prop_base_list[7]}, 	3,       	(u8*)&os.vars.runtime,          	"runtime",      	"Runtime from last reset (sec)",	VAR_TYPE_U32,   	1,        	7,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[8], 	(void*)&sofi_prop_base_list[8]}, 	2,       	(u8*)&os.vars.runtime_total,    	"runtime_total",	"Total runtime (sec)",          	VAR_TYPE_U32,   	1,        	8,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[9], 	(void*)&sofi_prop_base_list[9]}, 	3,       	(u8*)&os.vars.cpu_load,         	"cpu_load",     	"CPU load (%)",                 	VAR_TYPE_FLOAT, 	1,        	9,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[10],	(void*)&sofi_prop_base_list[10]},	2,       	(u8*)&os.vars.uniq_id,          	"uniq_id",      	"Unique ID number",             	VAR_TYPE_U16,   	12,       	10, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[11],	(void*)&sofi_prop_base_list[11]},	2,       	(u8*)&time.vars.year,           	"year",         	"Year",                         	VAR_TYPE_U16,   	1,        	11, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[12],	(void*)&sofi_prop_base_list[12]},	2,       	(u8*)&time.vars.month,          	"month",        	"Month [1..12]",                	VAR_TYPE_U16,   	1,        	12, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[13],	(void*)&sofi_prop_base_list[13]},	2,       	(u8*)&time.vars.day,            	"day",          	"Day [1..31]",                  	VAR_TYPE_U16,   	1,        	13, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[14],	(void*)&sofi_prop_base_list[14]},	2,       	(u8*)&time.vars.hour,           	"hour",         	"Hour [0..23]",                 	VAR_TYPE_U16,   	1,        	14, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[15],	(void*)&sofi_prop_base_list[15]},	2,       	(u8*)&time.vars.minute,         	"minute",       	"Minute [0..59]",               	VAR_TYPE_U16,   	1,        	15, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[16],	(void*)&sofi_prop_base_list[16]},	2,       	(u8*)&time.vars.sec,            	"sec",          	"Sec [0..59]",                  	VAR_TYPE_U16,   	1,        	16, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[17],	(void*)&sofi_prop_base_list[17]},	2,       	(u8*)&time.vars.msec,           	"msec",         	"Msec [0..999]",                	VAR_TYPE_U16,   	1,        	17, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[18],	(void*)&sofi_prop_base_list[18]},	2,       	(u8*)&time.vars.unix,           	"unix",         	"Unix timestamp",               	VAR_TYPE_U32,   	1,        	18, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[19],	(void*)&sofi_prop_base_list[19]},	2,       	(u8*)&time.vars.utc,            	"utc",          	"Utc [-12..14]",                	VAR_TYPE_S16,   	1,        	19, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[20],	(void*)&sofi_prop_base_list[20]},	2,       	(u8*)&device.vars.device_type,  	"device_type",  	"Device type code",             	VAR_TYPE_U16,   	1,        	20, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[21],	(void*)&sofi_prop_base_list[21]},	3,       	(u8*)&device.vars.device_name,  	"device_name",  	"Device name",                  	VAR_TYPE_CHAR,  	40,       	21, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[22],	(void*)&sofi_prop_base_list[22]},	2,       	(u8*)&device.vars.board_ver,    	"board_ver",    	"Hardware version",             	VAR_TYPE_U16,   	1,        	22, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[23],	(void*)&sofi_prop_base_list[23]},	3,       	(u8*)&device.vars.serial,       	"serial",       	"Serial number",                	VAR_TYPE_U32,   	1,        	23, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[30],	(void*)&sofi_prop_base_list[24]},	3,       	(u8*)&device.vars.configuration,	"configuration",	"Hardware configuration",       	VAR_TYPE_CHAR,  	40,       	24, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[24],	(void*)&sofi_prop_base_list[25]},	2,       	(u8*)&device.vars.temperature,  	"temperature",  	"Temperature (°C)",             	VAR_TYPE_FLOAT, 	1,        	25, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[25],	(void*)&sofi_prop_base_list[26]},	2,       	(u8*)&device.vars.v_bat,        	"v_bat",        	"Battery voltage (V)",          	VAR_TYPE_FLOAT, 	1,        	26, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[26],	(void*)&sofi_prop_base_list[27]},	2,       	(u8*)&device.vars.v_pwr,        	"v_pwr",        	"Power voltage (V)",            	VAR_TYPE_FLOAT, 	1,        	27, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[27],	(void*)&sofi_prop_base_list[28]},	3,       	(u8*)&modbus.vars.addr,         	"addr",         	"Device ModBUS address",        	VAR_TYPE_U16,   	1,        	28, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[28],	(void*)&sofi_prop_base_list[29]},	2,       	(u8*)&service.vars.rtc_state,   	"rtc_state",    	"RTC service status",           	VAR_TYPE_U32,   	1,        	29, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[31],	(void*)&sofi_prop_base_list[30]},	2,       	(u8*)&service.vars.adc_state,   	"adc_state",    	"ADC service status",           	VAR_TYPE_U32,   	1,        	30, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[32],	(void*)&sofi_prop_base_list[31]},	2,       	(u8*)&service.vars.rs485_state, 	"rs485_state",  	"RS485 service status",         	VAR_TYPE_U32,   	1,        	31, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[33],	(void*)&sofi_prop_base_list[32]},	2,       	(u8*)&service.vars.dbg_state,   	"dbg_state",    	"Debug service status",         	VAR_TYPE_U32,   	1,        	32, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[29],	(void*)&sofi_prop_base_list[33]},	2,       	(u8*)&test.vars.arr_u8,         	"arr_u8",       	"None",                         	VAR_TYPE_U8,    	20,       	33, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[34],	(void*)&sofi_prop_base_list[34]},	2,       	(u8*)&test.vars.arr_u16,        	"arr_u16",      	"None",                         	VAR_TYPE_U16,   	20,       	34, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[35],	(void*)&sofi_prop_base_list[35]},	2,       	(u8*)&test.vars.arr_u32,        	"arr_u32",      	"None",                         	VAR_TYPE_U32,   	20,       	35, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[36],	(void*)&sofi_prop_base_list[36]},	2,       	(u8*)&test.vars.arr_u64,        	"arr_u64",      	"None",                         	VAR_TYPE_U64,   	20,       	36, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[37],	(void*)&sofi_prop_base_list[37]},	2,       	(u8*)&test.vars.arr_s8,         	"arr_s8",       	"None",                         	VAR_TYPE_S8,    	20,       	37, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[38],	(void*)&sofi_prop_base_list[38]},	2,       	(u8*)&test.vars.arr_s16,        	"arr_s16",      	"None",                         	VAR_TYPE_S16,   	20,       	38, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[39],	(void*)&sofi_prop_base_list[39]},	2,       	(u8*)&test.vars.arr_s32,        	"arr_s32",      	"None",                         	VAR_TYPE_S32,   	20,       	39, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[40],	(void*)&sofi_prop_base_list[40]},	2,       	(u8*)&test.vars.arr_s64,        	"arr_s64",      	"None",                         	VAR_TYPE_S64,   	20,       	40, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[41],	(void*)&sofi_prop_base_list[41]},	2,       	(u8*)&test.vars.arr_float,      	"arr_float",    	"None",                         	VAR_TYPE_FLOAT, 	20,       	41, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[42],	(void*)&sofi_prop_base_list[42]},	2,       	(u8*)&test.vars.arr_double,     	"arr_double",   	"None",                         	VAR_TYPE_DOUBLE,	20,       	42, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[43],	(void*)&sofi_prop_base_list[43]},	2,       	(u8*)&test.vars.arr_char,       	"arr_char",     	"None",                         	VAR_TYPE_CHAR,  	20,       	43, 	0},
};

const sofi_prop_mdb_t sofi_prop_mdb_list[SOFI_PROP_MDB_REG_NUM]={
//prop          	header_next                    	header_base                     	mdb_addr 	
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[0]}, 	0},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[1]}, 	4},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[0],	(void*)&sofi_prop_base_list[2]}, 	5},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[3]}, 	6},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[4]}, 	26},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[5]}, 	46},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[1],	(void*)&sofi_prop_base_list[6]}, 	47},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[3],	(void*)&sofi_prop_base_list[7]}, 	48},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[8]}, 	50},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[4],	(void*)&sofi_prop_base_list[9]}, 	52},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[10]},	54},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[11]},	66},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[12]},	67},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[13]},	68},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[14]},	69},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[15]},	70},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[16]},	71},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[17]},	72},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[18]},	73},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[19]},	75},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[20]},	76},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[5],	(void*)&sofi_prop_base_list[21]},	77},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[22]},	97},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[6],	(void*)&sofi_prop_base_list[23]},	98},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[25]},	100},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[26]},	102},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[27]},	104},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[2],	(void*)&sofi_prop_base_list[28]},	106},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[29]},	107},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[33]},	110},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[7],	(void*)&sofi_prop_base_list[24]},	120},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[30]},	140},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[31]},	142},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[32]},	144},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[34]},	146},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[35]},	166},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[36]},	206},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[37]},	286},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[38]},	296},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[39]},	316},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[40]},	356},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[41]},	436},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[42]},	476},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[43]},	556},
};

const sofi_prop_range_t sofi_prop_range_list[SOFI_PROP_RANGE_REG_NUM]={
//prop 	header_next 	header_base 	p_def 	p_min 	p_max 	
};

const sofi_prop_save_t sofi_prop_save_list[SOFI_PROP_SAVE_REG_NUM]={
//prop           	header_next 	header_base                     	save_addr 	
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[2]}, 	0},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[6]}, 	2},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[28]},	4},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[7]}, 	6},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[9]}, 	10},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[21]},	14},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[23]},	54},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[24]},	58},
};

const sofi_prop_access_t sofi_prop_access_list[SOFI_PROP_ACCESS_REG_NUM]={
//prop 	header_next 	header_base 	access_lvl 	access_en_timer_ms 	
};

//#generator_message{"msg":"sofi_properties", "action":"insert_end"}
