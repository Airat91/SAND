/*
 * File:        regs_sand_ai.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global structs of SOFI module
 * Revision history: 0.1
 */

#include "regs_sand_ai.h"

//#generator_message{"msg":"sand_properties", "action":"insert_start"}
// This part of file generated automatically, don't change it

//Global structs declaration
os_struct      	os      	 = {0};
time_struct    	time    	 = {0};
device_struct  	device  	 = {0};
modbus_struct  	modbus  	 = {0};
service_struct 	service 	 = {0};
ai_struct      	ai      	 = {0};
test_struct    	test    	 = {0};

//range_const declaration
static const u16   	u16_1                = 1;
static const u16   	u16_12               = 12;
static const u16   	u16_31               = 31;
static const u16   	u16_0                = 0;
static const u16   	u16_23               = 23;
static const u16   	u16_59               = 59;
static const u16   	u16_999              = 999;
static const s16   	s16__12              = -12;
static const s16   	s16_14               = 14;
static const float 	float_16_0f_16_0f[]  = {16.0f,16.0f};
static const float 	float_0_0f_0_0f[]    = {0.0f,0.0f};

//Property lists
const sand_prop_base_t sand_prop_base_list[SAND_PROP_BASE_REG_NUM]={
//prop           	header_next                    	header_base                     	prop_num 	p_value                          	name             	description                     	type            	array_len 	ind 	read_only 	
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[0], 	(void*)&sand_prop_base_list[0]}, 	2,       	(u8*)&os.vars.os_version,        	"os_version",    	"Software version",             	VAR_TYPE_U16,   	4,        	0,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[1], 	(void*)&sand_prop_base_list[1]}, 	2,       	(u8*)&os.vars.num_of_vars,       	"num_of_vars",   	"Total registers in project",   	VAR_TYPE_U16,   	1,        	1,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[2], 	(void*)&sand_prop_base_list[2]}, 	3,       	(u8*)&os.vars.lang,              	"lang",          	"Interface language",           	VAR_TYPE_U16,   	1,        	2,  	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[3], 	(void*)&sand_prop_base_list[3]}, 	2,       	(u8*)&os.vars.build,             	"build",         	"Build info",                   	VAR_TYPE_CHAR,  	40,       	3,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[4], 	(void*)&sand_prop_base_list[4]}, 	2,       	(u8*)&os.vars.build_date,        	"build_date",    	"Date of SW building",          	VAR_TYPE_CHAR,  	20,       	4,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[5], 	(void*)&sand_prop_base_list[5]}, 	2,       	(u8*)&os.vars.command,           	"command",       	"Command register",             	VAR_TYPE_U16,   	1,        	5,  	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[6], 	(void*)&sand_prop_base_list[6]}, 	3,       	(u8*)&os.vars.reset_num,         	"reset_num",     	"Number of resets",             	VAR_TYPE_U16,   	1,        	6,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[7], 	(void*)&sand_prop_base_list[7]}, 	2,       	(u8*)&os.vars.runtime,           	"runtime",       	"Runtime from last reset (sec)",	VAR_TYPE_U32,   	1,        	7,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[8], 	(void*)&sand_prop_base_list[8]}, 	2,       	(u8*)&os.vars.runtime_total,     	"runtime_total", 	"Total runtime (sec)",          	VAR_TYPE_U32,   	1,        	8,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[9], 	(void*)&sand_prop_base_list[9]}, 	3,       	(u8*)&os.vars.cpu_load,          	"cpu_load",      	"CPU load (%)",                 	VAR_TYPE_FLOAT, 	1,        	9,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[10],	(void*)&sand_prop_base_list[10]},	2,       	(u8*)&time.vars.year,            	"year",          	"Year",                         	VAR_TYPE_U16,   	1,        	10, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[11],	(void*)&sand_prop_base_list[11]},	3,       	(u8*)&time.vars.month,           	"month",         	"Month [1..12]",                	VAR_TYPE_U16,   	1,        	11, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[12],	(void*)&sand_prop_base_list[12]},	3,       	(u8*)&time.vars.day,             	"day",           	"Day [1..31]",                  	VAR_TYPE_U16,   	1,        	12, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[13],	(void*)&sand_prop_base_list[13]},	3,       	(u8*)&time.vars.hour,            	"hour",          	"Hour [0..23]",                 	VAR_TYPE_U16,   	1,        	13, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[14],	(void*)&sand_prop_base_list[14]},	3,       	(u8*)&time.vars.minute,          	"minute",        	"Minute [0..59]",               	VAR_TYPE_U16,   	1,        	14, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[15],	(void*)&sand_prop_base_list[15]},	3,       	(u8*)&time.vars.sec,             	"sec",           	"Sec [0..59]",                  	VAR_TYPE_U16,   	1,        	15, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[16],	(void*)&sand_prop_base_list[16]},	3,       	(u8*)&time.vars.msec,            	"msec",          	"Msec [0..999]",                	VAR_TYPE_U16,   	1,        	16, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[17],	(void*)&sand_prop_base_list[17]},	2,       	(u8*)&time.vars.unix,            	"unix",          	"Unix timestamp",               	VAR_TYPE_U32,   	1,        	17, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[18],	(void*)&sand_prop_base_list[18]},	3,       	(u8*)&time.vars.utc,             	"utc",           	"Utc [-12..14]",                	VAR_TYPE_S16,   	1,        	18, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[19],	(void*)&sand_prop_base_list[19]},	2,       	(u8*)&device.vars.device_name,   	"device_name",   	"Device name",                  	VAR_TYPE_CHAR,  	40,       	19, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[20],	(void*)&sand_prop_base_list[20]},	3,       	(u8*)&device.vars.user_name,     	"user_name",     	"User device name",             	VAR_TYPE_CHAR,  	40,       	20, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[21],	(void*)&sand_prop_base_list[21]},	3,       	(u8*)&device.vars.configuration, 	"configuration", 	"Hardware configuration",       	VAR_TYPE_CHAR,  	40,       	21, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[22],	(void*)&sand_prop_base_list[22]},	2,       	(u8*)&device.vars.mcu_info,      	"mcu_info",      	"MCU information",              	VAR_TYPE_CHAR,  	40,       	22, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[23],	(void*)&sand_prop_base_list[23]},	2,       	(u8*)&device.vars.mcu_id,        	"mcu_id",        	"Unique MCU ID number",         	VAR_TYPE_U8,    	12,       	23, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[24],	(void*)&sand_prop_base_list[24]},	2,       	(u8*)&device.vars.device_type,   	"device_type",   	"Device type code",             	VAR_TYPE_U16,   	1,        	24, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[25],	(void*)&sand_prop_base_list[25]},	3,       	(u8*)&device.vars.board_ver,     	"board_ver",     	"Hardware version",             	VAR_TYPE_U16,   	1,        	25, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[26],	(void*)&sand_prop_base_list[26]},	3,       	(u8*)&device.vars.serial,        	"serial",        	"Serial number",                	VAR_TYPE_U32,   	1,        	26, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[27],	(void*)&sand_prop_base_list[27]},	2,       	(u8*)&device.vars.temperature,   	"temperature",   	"Temperature (°C)",             	VAR_TYPE_FLOAT, 	1,        	27, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[28],	(void*)&sand_prop_base_list[28]},	2,       	(u8*)&device.vars.v_bat,         	"v_bat",         	"Battery voltage (V)",          	VAR_TYPE_FLOAT, 	1,        	28, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[29],	(void*)&sand_prop_base_list[29]},	2,       	(u8*)&device.vars.v_pwr,         	"v_pwr",         	"Power voltage (V)",            	VAR_TYPE_FLOAT, 	1,        	29, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[30],	(void*)&sand_prop_base_list[30]},	2,       	(u8*)&device.vars.vref_int,      	"vref_int",      	"Vref internal (V)",            	VAR_TYPE_FLOAT, 	1,        	30, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[31],	(void*)&sand_prop_base_list[31]},	2,       	(u8*)&device.vars.vref_ext,      	"vref_ext",      	"Vref external (V)",            	VAR_TYPE_FLOAT, 	1,        	31, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[32],	(void*)&sand_prop_base_list[32]},	3,       	(u8*)&modbus.vars.addr,          	"addr",          	"Device ModBUS address",        	VAR_TYPE_U16,   	1,        	32, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[33],	(void*)&sand_prop_base_list[33]},	2,       	(u8*)&service.vars.rtc_state,    	"rtc_state",     	"RTC service status",           	VAR_TYPE_U32,   	1,        	33, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[34],	(void*)&sand_prop_base_list[34]},	2,       	(u8*)&service.vars.adc_int_state,	"adc_int_state", 	"ADC_INT service status",       	VAR_TYPE_U32,   	1,        	34, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[35],	(void*)&sand_prop_base_list[35]},	2,       	(u8*)&service.vars.rs485_state,  	"rs485_state",   	"RS-485 service status",        	VAR_TYPE_U32,   	1,        	35, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[36],	(void*)&sand_prop_base_list[36]},	2,       	(u8*)&service.vars.dbg_state,    	"dbg_state",     	"Debug service status",         	VAR_TYPE_U32,   	1,        	36, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[37],	(void*)&sand_prop_base_list[37]},	2,       	(u8*)&service.vars.ai_state,     	"ai_state",      	"AI service status",            	VAR_TYPE_U32,   	1,        	37, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[38],	(void*)&sand_prop_base_list[38]},	2,       	(u8*)&ai.vars.ai_value,          	"ai_value",      	"Measured value",               	VAR_TYPE_FLOAT, 	2,        	38, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[39],	(void*)&sand_prop_base_list[39]},	4,       	(u8*)&ai.vars.ai_calib_a,        	"ai_calib_a",    	"Calibration coef A",           	VAR_TYPE_FLOAT, 	2,        	39, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[40],	(void*)&sand_prop_base_list[40]},	4,       	(u8*)&ai.vars.ai_calib_b,        	"ai_calib_b",    	"Calibration coef B",           	VAR_TYPE_FLOAT, 	2,        	40, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[41],	(void*)&sand_prop_base_list[41]},	2,       	(u8*)&ai.vars.ai_adc,            	"ai_adc",        	"ADC raw data",                 	VAR_TYPE_U16,   	2,        	41, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[42],	(void*)&sand_prop_base_list[42]},	2,       	(u8*)&ai.vars.ai_sample_rate,    	"ai_sample_rate",	"Samples per second",           	VAR_TYPE_FLOAT, 	2,        	42, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[43],	(void*)&sand_prop_base_list[43]},	2,       	(u8*)&test.vars.arr_u8,          	"arr_u8",        	"None",                         	VAR_TYPE_U8,    	20,       	43, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[44],	(void*)&sand_prop_base_list[44]},	2,       	(u8*)&test.vars.arr_u16,         	"arr_u16",       	"None",                         	VAR_TYPE_U16,   	20,       	44, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[45],	(void*)&sand_prop_base_list[45]},	2,       	(u8*)&test.vars.arr_u32,         	"arr_u32",       	"None",                         	VAR_TYPE_U32,   	20,       	45, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[46],	(void*)&sand_prop_base_list[46]},	2,       	(u8*)&test.vars.arr_u64,         	"arr_u64",       	"None",                         	VAR_TYPE_U64,   	20,       	46, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[47],	(void*)&sand_prop_base_list[47]},	2,       	(u8*)&test.vars.arr_s8,          	"arr_s8",        	"None",                         	VAR_TYPE_S8,    	20,       	47, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[48],	(void*)&sand_prop_base_list[48]},	2,       	(u8*)&test.vars.arr_s16,         	"arr_s16",       	"None",                         	VAR_TYPE_S16,   	20,       	48, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[49],	(void*)&sand_prop_base_list[49]},	2,       	(u8*)&test.vars.arr_s32,         	"arr_s32",       	"None",                         	VAR_TYPE_S32,   	20,       	49, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[50],	(void*)&sand_prop_base_list[50]},	2,       	(u8*)&test.vars.arr_s64,         	"arr_s64",       	"None",                         	VAR_TYPE_S64,   	20,       	50, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[51],	(void*)&sand_prop_base_list[51]},	2,       	(u8*)&test.vars.arr_float,       	"arr_float",     	"None",                         	VAR_TYPE_FLOAT, 	20,       	51, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[52],	(void*)&sand_prop_base_list[52]},	2,       	(u8*)&test.vars.arr_double,      	"arr_double",    	"None",                         	VAR_TYPE_DOUBLE,	20,       	52, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[53],	(void*)&sand_prop_base_list[53]},	2,       	(u8*)&test.vars.arr_char,        	"arr_char",      	"None",                         	VAR_TYPE_CHAR,  	20,       	53, 	0},
};

const sand_prop_mdb_t sand_prop_mdb_list[SAND_PROP_MDB_REG_NUM]={
//prop          	header_next                     	header_base                     	mdb_addr 	   reg_name
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[0]}, 	0},         // os_version
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[1]}, 	4},         // num_of_vars
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[0], 	(void*)&sand_prop_base_list[2]}, 	5},         // lang
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[3]}, 	6},         // build
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[4]}, 	26},        // build_date
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[5]}, 	36},        // command
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[1], 	(void*)&sand_prop_base_list[6]}, 	37},        // reset_num
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[7]}, 	38},        // runtime
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[8]}, 	40},        // runtime_total
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[3], 	(void*)&sand_prop_base_list[9]}, 	42},        // cpu_load
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[10]},	44},        // year
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[0],	(void*)&sand_prop_base_list[11]},	45},        // month
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[1],	(void*)&sand_prop_base_list[12]},	46},        // day
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[2],	(void*)&sand_prop_base_list[13]},	47},        // hour
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[3],	(void*)&sand_prop_base_list[14]},	48},        // minute
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[4],	(void*)&sand_prop_base_list[15]},	49},        // sec
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[5],	(void*)&sand_prop_base_list[16]},	50},        // msec
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[17]},	51},        // unix
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[6],	(void*)&sand_prop_base_list[18]},	53},        // utc
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[19]},	54},        // device_name
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[4], 	(void*)&sand_prop_base_list[20]},	74},        // user_name
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[5], 	(void*)&sand_prop_base_list[21]},	94},        // configuration
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[22]},	114},       // mcu_info
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[23]},	134},       // mcu_id
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[24]},	140},       // device_type
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[6], 	(void*)&sand_prop_base_list[25]},	141},       // board_ver
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[7], 	(void*)&sand_prop_base_list[26]},	142},       // serial
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[27]},	144},       // temperature
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[28]},	146},       // v_bat
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[29]},	148},       // v_pwr
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[30]},	150},       // vref_int
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[31]},	152},       // vref_ext
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[2], 	(void*)&sand_prop_base_list[32]},	154},       // addr
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[33]},	155},       // rtc_state
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[34]},	157},       // adc_int_state
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[35]},	159},       // rs485_state
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[36]},	161},       // dbg_state
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[37]},	163},       // ai_state
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[38]},	165},       // ai_value
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[7],	(void*)&sand_prop_base_list[39]},	169},       // ai_calib_a
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[8],	(void*)&sand_prop_base_list[40]},	173},       // ai_calib_b
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[41]},	177},       // ai_adc
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[42]},	179},       // ai_sample_rate
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[43]},	183},       // arr_u8
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[44]},	193},       // arr_u16
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[45]},	213},       // arr_u32
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[46]},	253},       // arr_u64
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[47]},	333},       // arr_s8
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[48]},	343},       // arr_s16
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[49]},	363},       // arr_s32
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[50]},	403},       // arr_s64
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[51]},	483},       // arr_float
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[52]},	523},       // arr_double
{{SAND_PROP_MDB,	NULL,                           	(void*)&sand_prop_base_list[53]},	603},       // arr_char
};

const sand_prop_range_t sand_prop_range_list[SAND_PROP_RANGE_REG_NUM]={
//prop            	header_next                    	header_base                     	p_def                     	p_min           	p_max           	   reg_name
{{SAND_PROP_RANGE,	NULL,                          	(void*)&sand_prop_base_list[11]},	NULL,                     	(void*)&u16_1,  	(void*)&u16_12},   // month
{{SAND_PROP_RANGE,	NULL,                          	(void*)&sand_prop_base_list[12]},	NULL,                     	(void*)&u16_1,  	(void*)&u16_31},   // day
{{SAND_PROP_RANGE,	NULL,                          	(void*)&sand_prop_base_list[13]},	NULL,                     	(void*)&u16_0,  	(void*)&u16_23},   // hour
{{SAND_PROP_RANGE,	NULL,                          	(void*)&sand_prop_base_list[14]},	NULL,                     	(void*)&u16_0,  	(void*)&u16_59},   // minute
{{SAND_PROP_RANGE,	NULL,                          	(void*)&sand_prop_base_list[15]},	NULL,                     	(void*)&u16_0,  	(void*)&u16_59},   // sec
{{SAND_PROP_RANGE,	NULL,                          	(void*)&sand_prop_base_list[16]},	NULL,                     	(void*)&u16_0,  	(void*)&u16_999},  // msec
{{SAND_PROP_RANGE,	NULL,                          	(void*)&sand_prop_base_list[18]},	NULL,                     	(void*)&s16__12,	(void*)&s16_14},   // utc
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[8],	(void*)&sand_prop_base_list[39]},	(void*)&float_16_0f_16_0f,	NULL,           	NULL},             // ai_calib_a
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[9],	(void*)&sand_prop_base_list[40]},	(void*)&float_0_0f_0_0f,  	NULL,           	NULL},             // ai_calib_b
};

const sand_prop_save_t sand_prop_save_list[SAND_PROP_SAVE_REG_NUM]={
//prop           	header_next 	header_base                     	save_addr 	busy 	   reg_name
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[2]}, 	0,        	0},     // lang
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[6]}, 	2,        	0},     // reset_num
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[32]},	4,        	0},     // addr
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[9]}, 	6,        	0},     // cpu_load
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[20]},	10,       	0},     // user_name
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[21]},	50,       	0},     // configuration
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[25]},	90,       	0},     // board_ver
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[26]},	92,       	0},     // serial
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[39]},	96,       	0},     // ai_calib_a
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[40]},	104,      	0},     // ai_calib_b
};

const sand_prop_access_t sand_prop_access_list[SAND_PROP_ACCESS_REG_NUM]={
//prop 	header_next 	header_base 	access_lvl 	access_en_timer_ms 	   reg_name
};

//#generator_message{"msg":"sand_properties", "action":"insert_end"}
