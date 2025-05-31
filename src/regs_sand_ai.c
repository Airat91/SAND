/*
 * File:        regs_sand_ai.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global structs of SOFI module
 * Revision history: 0.1
 */

#include "regs_sand_ai.h"

//#generator_message{"msg":"sofi_properties", "action":"insert_start"}
// This part of file generated automatically, don't change it
os_struct     	os;
time_struct   	time;
device_struct 	device;
modbus_struct 	modbus;

const sofi_prop_base_t sofi_prop_base_list[SOFI_PROP_BASE_REG_NUM]={
//prop           	header_next                    	header_base                     	prop_num 	p_value                         	name            	description                     	type           	array_len 	ind 	read_only 	
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[0], 	(void*)&sofi_prop_base_list[0]}, 	2,       	(u8*)&os.vars.os_version,       	"os_version",   	"Software version",             	VAR_TYPE_U8,   	4,        	0,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[1], 	(void*)&sofi_prop_base_list[1]}, 	2,       	(u8*)&os.vars.num_of_vars,      	"num_of_vars",  	"Total vars in project",        	VAR_TYPE_U16,  	1,        	1,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[2], 	(void*)&sofi_prop_base_list[2]}, 	3,       	(u8*)&os.vars.lang,             	"lang",         	"Interface language",           	VAR_TYPE_U8,   	1,        	2,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[3], 	(void*)&sofi_prop_base_list[3]}, 	3,       	(u8*)&os.vars.build,            	"build",        	"Build info",                   	VAR_TYPE_CHAR, 	40,       	3,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[4], 	(void*)&sofi_prop_base_list[4]}, 	2,       	(u8*)&os.vars.command,          	"command",      	"Command register",             	VAR_TYPE_U16,  	1,        	4,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[5], 	(void*)&sofi_prop_base_list[5]}, 	3,       	(u8*)&os.vars.reset_num,        	"reset_num",    	"Number of resets",             	VAR_TYPE_U16,  	1,        	5,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[6], 	(void*)&sofi_prop_base_list[6]}, 	3,       	(u8*)&os.vars.runtime,          	"runtime",      	"Runtime from last reset (sec)",	VAR_TYPE_U32,  	1,        	6,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[7], 	(void*)&sofi_prop_base_list[7]}, 	2,       	(u8*)&os.vars.runtime_total,    	"runtime_total",	"Total runtime (sec)",          	VAR_TYPE_U32,  	1,        	7,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[8], 	(void*)&sofi_prop_base_list[8]}, 	3,       	(u8*)&os.vars.cpu_load,         	"cpu_load",     	"CPU load (%)",                 	VAR_TYPE_FLOAT,	1,        	8,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[9], 	(void*)&sofi_prop_base_list[9]}, 	2,       	(u8*)&os.vars.uniq_id,          	"uniq_id",      	"Unique ID number",             	VAR_TYPE_U8,   	12,       	9,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[10],	(void*)&sofi_prop_base_list[10]},	2,       	(u8*)&time.vars.year,           	"year",         	"Year",                         	VAR_TYPE_U16,  	1,        	10, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[11],	(void*)&sofi_prop_base_list[11]},	2,       	(u8*)&time.vars.month,          	"month",        	"Month [1..12]",                	VAR_TYPE_U8,   	1,        	11, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[12],	(void*)&sofi_prop_base_list[12]},	2,       	(u8*)&time.vars.day,            	"day",          	"Day [1..31]",                  	VAR_TYPE_U8,   	1,        	12, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[13],	(void*)&sofi_prop_base_list[13]},	2,       	(u8*)&time.vars.hour,           	"hour",         	"Hour [0..23]",                 	VAR_TYPE_U8,   	1,        	13, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[14],	(void*)&sofi_prop_base_list[14]},	2,       	(u8*)&time.vars.minute,         	"minute",       	"Minute [0..59]",               	VAR_TYPE_U8,   	1,        	14, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[15],	(void*)&sofi_prop_base_list[15]},	2,       	(u8*)&time.vars.sec,            	"sec",          	"Sec [0..59]",                  	VAR_TYPE_U8,   	1,        	15, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[16],	(void*)&sofi_prop_base_list[16]},	2,       	(u8*)&time.vars.msec,           	"msec",         	"Msec [0..999]",                	VAR_TYPE_U16,  	1,        	16, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[17],	(void*)&sofi_prop_base_list[17]},	2,       	(u8*)&time.vars.unix,           	"unix",         	"Unix timestamp",               	VAR_TYPE_U32,  	1,        	17, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[18],	(void*)&sofi_prop_base_list[18]},	2,       	(u8*)&time.vars.utc,            	"utc",          	"Utc [-12..14]",                	VAR_TYPE_S8,   	1,        	18, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[19],	(void*)&sofi_prop_base_list[19]},	2,       	(u8*)&device.vars.device_type,  	"device_type",  	"Device type code",             	VAR_TYPE_U8,   	1,        	19, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[20],	(void*)&sofi_prop_base_list[20]},	4,       	(u8*)&device.vars.device_name,  	"device_name",  	"Device name",                  	VAR_TYPE_CHAR, 	40,       	20, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[21],	(void*)&sofi_prop_base_list[21]},	2,       	(u8*)&device.vars.board_ver,    	"board_ver",    	"Hardware version",             	VAR_TYPE_U8,   	1,        	21, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[22],	(void*)&sofi_prop_base_list[22]},	3,       	(u8*)&device.vars.serial,       	"serial",       	"Serial number",                	VAR_TYPE_U32,  	1,        	22, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[23],	(void*)&sofi_prop_base_list[23]},	3,       	(u8*)&device.vars.configuration,	"configuration",	"Hardware configuration",       	VAR_TYPE_CHAR, 	40,       	23, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[24],	(void*)&sofi_prop_base_list[24]},	2,       	(u8*)&device.vars.temperature,  	"temperature",  	"Temperature (°C)",             	VAR_TYPE_FLOAT,	1,        	24, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[25],	(void*)&sofi_prop_base_list[25]},	2,       	(u8*)&device.vars.v_bat,        	"v_bat",        	"Battery voltage (V)",          	VAR_TYPE_FLOAT,	1,        	25, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[26],	(void*)&sofi_prop_base_list[26]},	2,       	(u8*)&device.vars.v_pwr,        	"v_pwr",        	"Power voltage (V)",            	VAR_TYPE_FLOAT,	1,        	26, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[27],	(void*)&sofi_prop_base_list[27]},	3,       	(u8*)&modbus.vars.addr,         	"addr",         	"Device ModBUS address",        	VAR_TYPE_U8,   	1,        	27, 	0},
};

const sofi_prop_mdb_t sofi_prop_mdb_list[SOFI_PROP_MDB_REG_NUM]={
//prop          	header_next                    	header_base                     	mdb_addr 	
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[0]}, 	0},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[1]}, 	2},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[0],	(void*)&sofi_prop_base_list[2]}, 	3},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[3],	(void*)&sofi_prop_base_list[3]}, 	4},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[4]}, 	24},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[1],	(void*)&sofi_prop_base_list[5]}, 	25},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[4],	(void*)&sofi_prop_base_list[6]}, 	26},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[7]}, 	28},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[5],	(void*)&sofi_prop_base_list[8]}, 	30},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[9]}, 	32},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[10]},	38},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[11]},	39},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[12]},	40},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[13]},	41},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[14]},	42},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[15]},	43},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[16]},	44},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[17]},	45},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[18]},	47},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[19]},	48},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[6],	(void*)&sofi_prop_base_list[20]},	49},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[21]},	69},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[7],	(void*)&sofi_prop_base_list[22]},	70},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[8],	(void*)&sofi_prop_base_list[23]},	72},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[24]},	92},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[25]},	94},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[26]},	96},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[2],	(void*)&sofi_prop_base_list[27]},	98},
};

const sofi_prop_range_t sofi_prop_range_list[SOFI_PROP_RANGE_REG_NUM]={
//prop 	header_next 	header_base 	p_def 	p_min 	p_max 	
};

const sofi_prop_save_t sofi_prop_save_list[SOFI_PROP_SAVE_REG_NUM]={
//prop           	header_next                      	header_base                     	save_addr 	
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[2]}, 	0},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[5]}, 	1},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[27]},	4},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[3]}, 	8},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[6]}, 	48},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[8]}, 	52},
{{SOFI_PROP_SAVE,	(void*)&sofi_prop_access_list[0],	(void*)&sofi_prop_base_list[20]},	56},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[22]},	96},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[23]},	100},
};

const sofi_prop_access_t sofi_prop_access_list[SOFI_PROP_ACCESS_REG_NUM]={
//prop             	header_next 	header_base                     	access_lvl       	access_en_timer_ms 	
{{SOFI_PROP_ACCESS,	NULL,       	(void*)&sofi_prop_base_list[20]},	ACC_LVL_USER_LOW,	0},
};

//#generator_message{"msg":"sofi_properties", "action":"insert_end"}
