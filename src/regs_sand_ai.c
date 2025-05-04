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
device_struct 	device;
modbus_struct 	modbus;

const sofi_prop_base_t sofi_prop_base_list[SOFI_PROP_BASE_REG_NUM]={
//prop           	header_next                    	header_base                     	prop_num 	p_value                                	name                   	description             	type           	array_len 	ind 	read_only 	
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[0], 	(void*)&sofi_prop_base_list[0]}, 	2,       	(u8*)&os.vars.os_version,              	"os_version",          	"Software version",     	VAR_TYPE_U8,   	4,        	0,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[1], 	(void*)&sofi_prop_base_list[1]}, 	2,       	(u8*)&os.vars.num_of_vars,             	"num_of_vars",         	"Total vars in project",	VAR_TYPE_U16,  	1,        	1,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[2], 	(void*)&sofi_prop_base_list[2]}, 	3,       	(u8*)&os.vars.lang,                    	"lang",                	"Interface language",   	VAR_TYPE_U8,   	1,        	2,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[3], 	(void*)&sofi_prop_base_list[3]}, 	3,       	(u8*)&os.vars.build,                   	"build",               	"Build info",           	VAR_TYPE_CHAR, 	40,       	3,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[4], 	(void*)&sofi_prop_base_list[4]}, 	2,       	(u8*)&os.vars.command,                 	"command",             	"None",                 	VAR_TYPE_U16,  	1,        	4,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[5], 	(void*)&sofi_prop_base_list[5]}, 	3,       	(u8*)&os.vars.reset_num,               	"reset_num",           	"None",                 	VAR_TYPE_U16,  	1,        	5,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[6], 	(void*)&sofi_prop_base_list[6]}, 	3,       	(u8*)&os.vars.runtime,                 	"runtime",             	"None",                 	VAR_TYPE_U32,  	1,        	6,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[7], 	(void*)&sofi_prop_base_list[7]}, 	2,       	(u8*)&os.vars.runtime_total,           	"runtime_total",       	"None",                 	VAR_TYPE_U32,  	1,        	7,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[8], 	(void*)&sofi_prop_base_list[8]}, 	3,       	(u8*)&os.vars.cpu_load,                	"cpu_load",            	"None",                 	VAR_TYPE_FLOAT,	1,        	8,  	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[9], 	(void*)&sofi_prop_base_list[9]}, 	2,       	(u8*)&os.vars.uniq_id,                 	"uniq_id",             	"None",                 	VAR_TYPE_U8,   	12,       	9,  	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[10],	(void*)&sofi_prop_base_list[10]},	2,       	(u8*)&device.vars.device_type,         	"device_type",         	"None",                 	VAR_TYPE_U8,   	1,        	10, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[11],	(void*)&sofi_prop_base_list[11]},	4,       	(u8*)&device.vars.device_name,         	"device_name",         	"None",                 	VAR_TYPE_CHAR, 	40,       	11, 	1},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[12],	(void*)&sofi_prop_base_list[12]},	2,       	(u8*)&device.vars.board_ver,           	"board_ver",           	"None",                 	VAR_TYPE_U8,   	1,        	12, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[13],	(void*)&sofi_prop_base_list[13]},	3,       	(u8*)&device.vars.serial_very_long_nam,	"serial_very_long_nam",	"None",                 	VAR_TYPE_U32,  	1,        	13, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[14],	(void*)&sofi_prop_base_list[14]},	3,       	(u8*)&device.vars.configuration,       	"configuration",       	"None",                 	VAR_TYPE_CHAR, 	40,       	14, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[15],	(void*)&sofi_prop_base_list[15]},	2,       	(u8*)&device.vars.temperature,         	"temperature",         	"None",                 	VAR_TYPE_FLOAT,	1,        	15, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[16],	(void*)&sofi_prop_base_list[16]},	2,       	(u8*)&device.vars.v_bat,               	"v_bat",               	"None",                 	VAR_TYPE_FLOAT,	1,        	16, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[17],	(void*)&sofi_prop_base_list[17]},	2,       	(u8*)&device.vars.v_pwr,               	"v_pwr",               	"None",                 	VAR_TYPE_FLOAT,	1,        	17, 	0},
{{SOFI_PROP_BASE,	(void*)&sofi_prop_mdb_list[18],	(void*)&sofi_prop_base_list[18]},	3,       	(u8*)&modbus.vars.addr,                	"addr",                	"Device ModBUS address",	VAR_TYPE_U8,   	1,        	18, 	0},
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
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[6],	(void*)&sofi_prop_base_list[11]},	39},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[12]},	59},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[7],	(void*)&sofi_prop_base_list[13]},	60},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[8],	(void*)&sofi_prop_base_list[14]},	62},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[15]},	82},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[16]},	84},
{{SOFI_PROP_MDB,	NULL,                          	(void*)&sofi_prop_base_list[17]},	86},
{{SOFI_PROP_MDB,	(void*)&sofi_prop_save_list[2],	(void*)&sofi_prop_base_list[18]},	88},
};

const sofi_prop_range_t sofi_prop_range_list[SOFI_PROP_RANGE_REG_NUM]={
//prop 	header_next 	header_base 	p_def 	p_min 	p_max 	
};

const sofi_prop_save_t sofi_prop_save_list[SOFI_PROP_SAVE_REG_NUM]={
//prop           	header_next                      	header_base                     	save_addr 	
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[2]}, 	0},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[5]}, 	1},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[18]},	4},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[3]}, 	8},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[6]}, 	48},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[8]}, 	52},
{{SOFI_PROP_SAVE,	(void*)&sofi_prop_access_list[0],	(void*)&sofi_prop_base_list[11]},	56},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[13]},	96},
{{SOFI_PROP_SAVE,	NULL,                            	(void*)&sofi_prop_base_list[14]},	100},
};

const sofi_prop_access_t sofi_prop_access_list[SOFI_PROP_ACCESS_REG_NUM]={
//prop             	header_next 	header_base                     	access_lvl       	access_en_timer_ms 	
{{SOFI_PROP_ACCESS,	NULL,       	(void*)&sofi_prop_base_list[11]},	ACC_LVL_USER_LOW,	0},
};

//#generator_message{"msg":"sofi_properties", "action":"insert_end"}
