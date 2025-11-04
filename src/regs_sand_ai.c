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
debug_struct   	debug;
ai_struct      	ai;
test_struct    	test;

const sofi_prop_base_t sofi_prop_base_list[SOFI_PROP_BASE_REG_NUM]={
//prop           	header_next                    	header_base                     	prop_num 	p_value                          	name             	description                     	type            	array_len 	ind 	read_only 	
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[0], 	(void*)&sofi_prop_base_list[0]}, 	2,       	(u8*)&os.vars.os_version,        	"os_version",    	"Software version",             	VAR_TYPE_U16,   	4,        	0,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[1], 	(void*)&sofi_prop_base_list[1]}, 	2,       	(u8*)&os.vars.num_of_vars,       	"num_of_vars",   	"Total registers in project",   	VAR_TYPE_U16,   	1,        	1,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[2], 	(void*)&sofi_prop_base_list[2]}, 	3,       	(u8*)&os.vars.lang,              	"lang",          	"Interface language",           	VAR_TYPE_U16,   	1,        	2,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[3], 	(void*)&sofi_prop_base_list[3]}, 	2,       	(u8*)&os.vars.build,             	"build",         	"Build info",                   	VAR_TYPE_CHAR,  	40,       	3,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[4], 	(void*)&sofi_prop_base_list[4]}, 	2,       	(u8*)&os.vars.build_date,        	"build_date",    	"Date of SW building",          	VAR_TYPE_CHAR,  	40,       	4,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[5], 	(void*)&sofi_prop_base_list[5]}, 	2,       	(u8*)&os.vars.command,           	"command",       	"Command register",             	VAR_TYPE_U16,   	1,        	5,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[6], 	(void*)&sofi_prop_base_list[6]}, 	3,       	(u8*)&os.vars.reset_num,         	"reset_num",     	"Number of resets",             	VAR_TYPE_U16,   	1,        	6,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[7], 	(void*)&sofi_prop_base_list[7]}, 	2,       	(u8*)&os.vars.runtime,           	"runtime",       	"Runtime from last reset (sec)",	VAR_TYPE_U32,   	1,        	7,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[8], 	(void*)&sofi_prop_base_list[8]}, 	2,       	(u8*)&os.vars.runtime_total,     	"runtime_total", 	"Total runtime (sec)",          	VAR_TYPE_U32,   	1,        	8,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[9], 	(void*)&sofi_prop_base_list[9]}, 	3,       	(u8*)&os.vars.cpu_load,          	"cpu_load",      	"CPU load (%)",                 	VAR_TYPE_FLOAT, 	1,        	9,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[10],	(void*)&sofi_prop_base_list[10]},	2,       	(u8*)&time.vars.year,            	"year",          	"Year",                         	VAR_TYPE_U16,   	1,        	10, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[11],	(void*)&sofi_prop_base_list[11]},	2,       	(u8*)&time.vars.month,           	"month",         	"Month [1..12]",                	VAR_TYPE_U16,   	1,        	11, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[12],	(void*)&sofi_prop_base_list[12]},	2,       	(u8*)&time.vars.day,             	"day",           	"Day [1..31]",                  	VAR_TYPE_U16,   	1,        	12, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[13],	(void*)&sofi_prop_base_list[13]},	2,       	(u8*)&time.vars.hour,            	"hour",          	"Hour [0..23]",                 	VAR_TYPE_U16,   	1,        	13, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[14],	(void*)&sofi_prop_base_list[14]},	2,       	(u8*)&time.vars.minute,          	"minute",        	"Minute [0..59]",               	VAR_TYPE_U16,   	1,        	14, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[15],	(void*)&sofi_prop_base_list[15]},	2,       	(u8*)&time.vars.sec,             	"sec",           	"Sec [0..59]",                  	VAR_TYPE_U16,   	1,        	15, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[16],	(void*)&sofi_prop_base_list[16]},	2,       	(u8*)&time.vars.msec,            	"msec",          	"Msec [0..999]",                	VAR_TYPE_U16,   	1,        	16, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[17],	(void*)&sofi_prop_base_list[17]},	2,       	(u8*)&time.vars.unix,            	"unix",          	"Unix timestamp",               	VAR_TYPE_U32,   	1,        	17, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[18],	(void*)&sofi_prop_base_list[18]},	2,       	(u8*)&time.vars.utc,             	"utc",           	"Utc [-12..14]",                	VAR_TYPE_S16,   	1,        	18, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[19],	(void*)&sofi_prop_base_list[19]},	2,       	(u8*)&device.vars.device_name,   	"device_name",   	"Device name",                  	VAR_TYPE_CHAR,  	40,       	19, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[20],	(void*)&sofi_prop_base_list[20]},	3,       	(u8*)&device.vars.user_name,     	"user_name",     	"User device name",             	VAR_TYPE_CHAR,  	40,       	20, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[21],	(void*)&sofi_prop_base_list[21]},	3,       	(u8*)&device.vars.configuration, 	"configuration", 	"Hardware configuration",       	VAR_TYPE_CHAR,  	40,       	21, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[22],	(void*)&sofi_prop_base_list[22]},	2,       	(u8*)&device.vars.mcu_info,      	"mcu_info",      	"MCU information",              	VAR_TYPE_CHAR,  	40,       	22, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[23],	(void*)&sofi_prop_base_list[23]},	2,       	(u8*)&device.vars.mcu_id,        	"mcu_id",        	"Unique MCU ID number",         	VAR_TYPE_U8,    	12,       	23, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[24],	(void*)&sofi_prop_base_list[24]},	2,       	(u8*)&device.vars.device_type,   	"device_type",   	"Device type code",             	VAR_TYPE_U16,   	1,        	24, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[25],	(void*)&sofi_prop_base_list[25]},	3,       	(u8*)&device.vars.board_ver,     	"board_ver",     	"Hardware version",             	VAR_TYPE_U16,   	1,        	25, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[26],	(void*)&sofi_prop_base_list[26]},	3,       	(u8*)&device.vars.serial,        	"serial",        	"Serial number",                	VAR_TYPE_U32,   	1,        	26, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[27],	(void*)&sofi_prop_base_list[27]},	2,       	(u8*)&device.vars.temperature,   	"temperature",   	"Temperature (°C)",             	VAR_TYPE_FLOAT, 	1,        	27, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[28],	(void*)&sofi_prop_base_list[28]},	2,       	(u8*)&device.vars.v_bat,         	"v_bat",         	"Battery voltage (V)",          	VAR_TYPE_FLOAT, 	1,        	28, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[29],	(void*)&sofi_prop_base_list[29]},	2,       	(u8*)&device.vars.v_pwr,         	"v_pwr",         	"Power voltage (V)",            	VAR_TYPE_FLOAT, 	1,        	29, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[30],	(void*)&sofi_prop_base_list[30]},	2,       	(u8*)&device.vars.vref_int,      	"vref_int",      	"Vref internal (V)",            	VAR_TYPE_FLOAT, 	1,        	30, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[31],	(void*)&sofi_prop_base_list[31]},	2,       	(u8*)&device.vars.vref_ext,      	"vref_ext",      	"Vref external (V)",            	VAR_TYPE_FLOAT, 	1,        	31, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[32],	(void*)&sofi_prop_base_list[32]},	3,       	(u8*)&modbus.vars.addr,          	"addr",          	"Device ModBUS address",        	VAR_TYPE_U16,   	1,        	32, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[33],	(void*)&sofi_prop_base_list[33]},	2,       	(u8*)&service.vars.rtc_state,    	"rtc_state",     	"RTC service status",           	VAR_TYPE_U32,   	1,        	33, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[34],	(void*)&sofi_prop_base_list[34]},	2,       	(u8*)&service.vars.adc_int_state,	"adc_int_state", 	"ADC_INT service status",       	VAR_TYPE_U32,   	1,        	34, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[35],	(void*)&sofi_prop_base_list[35]},	2,       	(u8*)&service.vars.rs485_state,  	"rs485_state",   	"RS-485 service status",        	VAR_TYPE_U32,   	1,        	35, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[36],	(void*)&sofi_prop_base_list[36]},	2,       	(u8*)&service.vars.dbg_state,    	"dbg_state",     	"Debug service status",         	VAR_TYPE_U32,   	1,        	36, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[37],	(void*)&sofi_prop_base_list[37]},	2,       	(u8*)&service.vars.ai_state,     	"ai_state",      	"AI service status",            	VAR_TYPE_U32,   	1,        	37, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[38],	(void*)&sofi_prop_base_list[38]},	2,       	(u8*)&ai.vars.ai_value,          	"ai_value",      	"Measured value",               	VAR_TYPE_FLOAT, 	2,        	38, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[39],	(void*)&sofi_prop_base_list[39]},	3,       	(u8*)&ai.vars.ai_calib_a,        	"ai_calib_a",    	"Calibration coef A",           	VAR_TYPE_FLOAT, 	2,        	39, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[40],	(void*)&sofi_prop_base_list[40]},	3,       	(u8*)&ai.vars.ai_calib_b,        	"ai_calib_b",    	"Calibration coef B",           	VAR_TYPE_FLOAT, 	2,        	40, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[41],	(void*)&sofi_prop_base_list[41]},	2,       	(u8*)&ai.vars.ai_adc,            	"ai_adc",        	"ADC raw data",                 	VAR_TYPE_U16,   	2,        	41, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[42],	(void*)&sofi_prop_base_list[42]},	2,       	(u8*)&ai.vars.ai_sample_rate,    	"ai_sample_rate",	"Samples per second",           	VAR_TYPE_FLOAT, 	2,        	42, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[43],	(void*)&sofi_prop_base_list[43]},	2,       	(u8*)&test.vars.arr_u8,          	"arr_u8",        	"None",                         	VAR_TYPE_U8,    	20,       	43, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[44],	(void*)&sofi_prop_base_list[44]},	2,       	(u8*)&test.vars.arr_u16,         	"arr_u16",       	"None",                         	VAR_TYPE_U16,   	20,       	44, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[45],	(void*)&sofi_prop_base_list[45]},	2,       	(u8*)&test.vars.arr_u32,         	"arr_u32",       	"None",                         	VAR_TYPE_U32,   	20,       	45, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[46],	(void*)&sofi_prop_base_list[46]},	2,       	(u8*)&test.vars.arr_u64,         	"arr_u64",       	"None",                         	VAR_TYPE_U64,   	20,       	46, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[47],	(void*)&sofi_prop_base_list[47]},	2,       	(u8*)&test.vars.arr_s8,          	"arr_s8",        	"None",                         	VAR_TYPE_S8,    	20,       	47, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[48],	(void*)&sofi_prop_base_list[48]},	2,       	(u8*)&test.vars.arr_s16,         	"arr_s16",       	"None",                         	VAR_TYPE_S16,   	20,       	48, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[49],	(void*)&sofi_prop_base_list[49]},	2,       	(u8*)&test.vars.arr_s32,         	"arr_s32",       	"None",                         	VAR_TYPE_S32,   	20,       	49, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[50],	(void*)&sofi_prop_base_list[50]},	2,       	(u8*)&test.vars.arr_s64,         	"arr_s64",       	"None",                         	VAR_TYPE_S64,   	20,       	50, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[51],	(void*)&sofi_prop_base_list[51]},	2,       	(u8*)&test.vars.arr_float,       	"arr_float",     	"None",                         	VAR_TYPE_FLOAT, 	20,       	51, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[52],	(void*)&sofi_prop_base_list[52]},	2,       	(u8*)&test.vars.arr_double,      	"arr_double",    	"None",                         	VAR_TYPE_DOUBLE,	20,       	52, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[53],	(void*)&sofi_prop_base_list[53]},	2,       	(u8*)&test.vars.arr_char,        	"arr_char",      	"None",                         	VAR_TYPE_CHAR,  	20,       	53, 	0},
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
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[7]}, 	48},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[8]}, 	50},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[3],	(void*)&sofi_prop_base_list[9]}, 	52},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[10]},	54},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[11]},	55},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[12]},	56},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[13]},	57},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[14]},	58},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[15]},	59},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[16]},	60},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[17]},	61},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[18]},	63},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[19]},	64},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[4],	(void*)&sofi_prop_base_list[20]},	84},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[5],	(void*)&sofi_prop_base_list[21]},	104},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[22]},	124},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[23]},	144},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[24]},	150},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[6],	(void*)&sofi_prop_base_list[25]},	151},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[7],	(void*)&sofi_prop_base_list[26]},	152},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[27]},	154},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[28]},	156},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[29]},	158},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[30]},	160},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[31]},	162},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[2],	(void*)&sofi_prop_base_list[32]},	164},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[33]},	165},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[34]},	167},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[35]},	169},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[36]},	171},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[37]},	173},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[38]},	175},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[8],	(void*)&sofi_prop_base_list[39]},	179},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[9],	(void*)&sofi_prop_base_list[40]},	183},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[41]},	187},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[42]},	189},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[43]},	193},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[44]},	203},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[45]},	223},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[46]},	263},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[47]},	343},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[48]},	353},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[49]},	373},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[50]},	413},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[51]},	493},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[52]},	533},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[53]},	613},
};

const sofi_prop_range_t sofi_prop_range_list[SOFI_PROP_RANGE_REG_NUM]={
//prop 	header_next 	header_base 	p_def 	p_min 	p_max 	
};

const sofi_prop_save_t sofi_prop_save_list[SOFI_PROP_SAVE_REG_NUM]={
//prop           	header_next 	header_base                     	save_addr 	
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[2]}, 	0},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[6]}, 	2},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[32]},	4},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[9]}, 	6},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[20]},	10},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[21]},	50},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[25]},	90},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[26]},	92},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[39]},	96},
{{SOFI_PROP_SAVE,	NULL,       	(void*)&sofi_prop_base_list[40]},	104},
};

const sofi_prop_access_t sofi_prop_access_list[SOFI_PROP_ACCESS_REG_NUM]={
//prop 	header_next 	header_base 	access_lvl 	access_en_timer_ms 	
};

//#generator_message{"msg":"sofi_properties", "action":"insert_end"}
