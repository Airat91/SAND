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
device_struct  	device  	 = {0};
service_struct 	service 	 = {0};
time_struct    	time    	 = {0};
storage_struct 	storage 	 = {0};
modbus_struct  	modbus  	 = {0};
ai_struct      	ai      	 = {0};
test_struct    	test    	 = {0};

//range_const declaration
static const u16   	u16_0                = 0;
static const u16   	u16_1                = 1;
static const float 	float_0_0f           = 0.0f;
static const float 	float_16_0f          = 16.0f;
static const u16   	u16_12               = 12;
static const u16   	u16_31               = 31;
static const u16   	u16_23               = 23;
static const u16   	u16_59               = 59;
static const u16   	u16_999              = 999;
static const s16   	s16__12              = -12;
static const s16   	s16_14               = 14;
static const u16   	u16_10               = 10;
static const u16   	u16_1440             = 1440;
static const u16   	u16_255              = 255;
static const float 	float_16_0f_16_0f[]  = {16.0f,16.0f};
static const float 	float_0_0f_0_0f[]    = {0.0f,0.0f};

//Property lists
const sand_prop_base_t sand_prop_base_list[SAND_PROP_BASE_REG_NUM]={
//prop           	header_next                    	header_base                     	prop_num 	p_value                            	name              	description                                	type            	array_len 	ind 	read_only 	
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[0], 	(void*)&sand_prop_base_list[0]}, 	2,       	(u8*)&os.vars.os_version,          	"os_version",     	"Software version",                        	VAR_TYPE_U16,   	4,        	0,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[1], 	(void*)&sand_prop_base_list[1]}, 	2,       	(u8*)&os.vars.num_of_vars,         	"num_of_vars",    	"Total registers in project",              	VAR_TYPE_U16,   	1,        	1,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[2], 	(void*)&sand_prop_base_list[2]}, 	4,       	(u8*)&os.vars.lang,                	"lang",           	"Interface language",                      	VAR_TYPE_U16,   	1,        	2,  	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[3], 	(void*)&sand_prop_base_list[3]}, 	3,       	(u8*)&os.vars.command,             	"command",        	"Command register",                        	VAR_TYPE_U16,   	1,        	3,  	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[4], 	(void*)&sand_prop_base_list[4]}, 	3,       	(u8*)&os.vars.reset_num,           	"reset_num",      	"Number of resets",                        	VAR_TYPE_U16,   	1,        	4,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[5], 	(void*)&sand_prop_base_list[5]}, 	2,       	(u8*)&os.vars.runtime,             	"runtime",        	"Runtime from last reset (sec)",           	VAR_TYPE_U32,   	1,        	5,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[6], 	(void*)&sand_prop_base_list[6]}, 	2,       	(u8*)&os.vars.runtime_total,       	"runtime_total",  	"Total runtime (sec)",                     	VAR_TYPE_U32,   	1,        	6,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[7], 	(void*)&sand_prop_base_list[7]}, 	2,       	(u8*)&os.vars.cpu_load,            	"cpu_load",       	"CPU load (%)",                            	VAR_TYPE_FLOAT, 	1,        	7,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[8], 	(void*)&sand_prop_base_list[8]}, 	2,       	(u8*)&os.vars.build,               	"build",          	"Build info",                              	VAR_TYPE_CHAR,  	40,       	8,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[11],	(void*)&sand_prop_base_list[9]}, 	2,       	(u8*)&os.vars.build_date,          	"build_date",     	"Date of SW building",                     	VAR_TYPE_CHAR,  	20,       	9,  	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[16],	(void*)&sand_prop_base_list[10]},	2,       	(u8*)&device.vars.device_name,     	"device_name",    	"Device name",                             	VAR_TYPE_CHAR,  	40,       	10, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[18],	(void*)&sand_prop_base_list[11]},	3,       	(u8*)&device.vars.user_note,       	"user_note",      	"User device name",                        	VAR_TYPE_CHAR,  	40,       	11, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[19],	(void*)&sand_prop_base_list[12]},	3,       	(u8*)&device.vars.configuration,   	"configuration",  	"Hardware configuration",                  	VAR_TYPE_CHAR,  	40,       	12, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[20],	(void*)&sand_prop_base_list[13]},	2,       	(u8*)&device.vars.mcu_info,        	"mcu_info",       	"MCU information",                         	VAR_TYPE_CHAR,  	40,       	13, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[21],	(void*)&sand_prop_base_list[14]},	2,       	(u8*)&device.vars.mcu_id,          	"mcu_id",         	"Unique MCU ID number",                    	VAR_TYPE_U8,    	12,       	14, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[22],	(void*)&sand_prop_base_list[15]},	2,       	(u8*)&device.vars.device_type,     	"device_type",    	"Device type code",                        	VAR_TYPE_U16,   	1,        	15, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[23],	(void*)&sand_prop_base_list[16]},	3,       	(u8*)&device.vars.board_ver,       	"board_ver",      	"Hardware version",                        	VAR_TYPE_U16,   	1,        	16, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[24],	(void*)&sand_prop_base_list[17]},	3,       	(u8*)&device.vars.serial,          	"serial",         	"Serial number",                           	VAR_TYPE_U32,   	1,        	17, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[25],	(void*)&sand_prop_base_list[18]},	2,       	(u8*)&device.vars.temperature,     	"temperature",    	"Temperature (°C)",                        	VAR_TYPE_FLOAT, 	1,        	18, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[26],	(void*)&sand_prop_base_list[19]},	2,       	(u8*)&device.vars.v_bat,           	"v_bat",          	"Battery voltage (V)",                     	VAR_TYPE_FLOAT, 	1,        	19, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[27],	(void*)&sand_prop_base_list[20]},	2,       	(u8*)&device.vars.v_pwr,           	"v_pwr",          	"Power voltage (V)",                       	VAR_TYPE_FLOAT, 	1,        	20, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[28],	(void*)&sand_prop_base_list[21]},	2,       	(u8*)&device.vars.vref_int,        	"vref_int",       	"Vref internal (V)",                       	VAR_TYPE_FLOAT, 	1,        	21, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[29],	(void*)&sand_prop_base_list[22]},	2,       	(u8*)&device.vars.vref_ext,        	"vref_ext",       	"Vref external (V)",                       	VAR_TYPE_FLOAT, 	1,        	22, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[30],	(void*)&sand_prop_base_list[23]},	4,       	(u8*)&device.vars.tmpr_add,        	"tmpr_add",       	"Temperature calibration additional coeff",	VAR_TYPE_FLOAT, 	1,        	23, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[31],	(void*)&sand_prop_base_list[24]},	4,       	(u8*)&device.vars.v_pwr_mul,       	"v_pwr_mul",      	"Power voltage calibration multiple coeff",	VAR_TYPE_FLOAT, 	1,        	24, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[32],	(void*)&sand_prop_base_list[25]},	4,       	(u8*)&device.vars.v_pwr_add,       	"v_pwr_add",      	"Power voltage calibration additional coe",	VAR_TYPE_FLOAT, 	1,        	25, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[33],	(void*)&sand_prop_base_list[26]},	4,       	(u8*)&device.vars.vref_sel,        	"vref_sel",       	"Select Vref source (0-Vref_int, 1-Vref_e",	VAR_TYPE_U16,   	1,        	26, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[34],	(void*)&sand_prop_base_list[27]},	2,       	(u8*)&service.vars.rtc_state,      	"rtc_state",      	"RTC service status",                      	VAR_TYPE_U32,   	1,        	27, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[35],	(void*)&sand_prop_base_list[28]},	2,       	(u8*)&service.vars.adc_int_state,  	"adc_int_state",  	"ADC_INT service status",                  	VAR_TYPE_U32,   	1,        	28, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[36],	(void*)&sand_prop_base_list[29]},	2,       	(u8*)&service.vars.rs485_state,    	"rs485_state",    	"RS-485 service status",                   	VAR_TYPE_U32,   	1,        	29, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[37],	(void*)&sand_prop_base_list[30]},	2,       	(u8*)&service.vars.dbg_state,      	"dbg_state",      	"Debug service status",                    	VAR_TYPE_U32,   	1,        	30, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[38],	(void*)&sand_prop_base_list[31]},	2,       	(u8*)&service.vars.ai_state,       	"ai_state",       	"AI service status",                       	VAR_TYPE_U32,   	1,        	31, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[40],	(void*)&sand_prop_base_list[32]},	2,       	(u8*)&time.vars.year,              	"year",           	"Year",                                    	VAR_TYPE_U16,   	1,        	32, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[41],	(void*)&sand_prop_base_list[33]},	3,       	(u8*)&time.vars.month,             	"month",          	"Month [1..12]",                           	VAR_TYPE_U16,   	1,        	33, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[42],	(void*)&sand_prop_base_list[34]},	3,       	(u8*)&time.vars.day,               	"day",            	"Day [1..31]",                             	VAR_TYPE_U16,   	1,        	34, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[43],	(void*)&sand_prop_base_list[35]},	3,       	(u8*)&time.vars.hour,              	"hour",           	"Hour [0..23]",                            	VAR_TYPE_U16,   	1,        	35, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[44],	(void*)&sand_prop_base_list[36]},	3,       	(u8*)&time.vars.minute,            	"minute",         	"Minute [0..59]",                          	VAR_TYPE_U16,   	1,        	36, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[45],	(void*)&sand_prop_base_list[37]},	3,       	(u8*)&time.vars.sec,               	"sec",            	"Sec [0..59]",                             	VAR_TYPE_U16,   	1,        	37, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[46],	(void*)&sand_prop_base_list[38]},	3,       	(u8*)&time.vars.msec,              	"msec",           	"Msec [0..999]",                           	VAR_TYPE_U16,   	1,        	38, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[47],	(void*)&sand_prop_base_list[39]},	3,       	(u8*)&time.vars.utc,               	"utc",            	"Utc [-12..14]",                           	VAR_TYPE_S16,   	1,        	39, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[48],	(void*)&sand_prop_base_list[40]},	2,       	(u8*)&time.vars.unix,              	"unix",           	"Unix timestamp",                          	VAR_TYPE_U32,   	1,        	40, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[50],	(void*)&sand_prop_base_list[41]},	2,       	(u8*)&storage.vars.erase_cnt,      	"erase_cnt",      	"Storage FLASH erase counter",             	VAR_TYPE_U32,   	1,        	41, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[51],	(void*)&sand_prop_base_list[42]},	4,       	(u8*)&storage.vars.autosave_en,    	"autosave_en",    	"Enable autosave",                         	VAR_TYPE_U16,   	1,        	42, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[52],	(void*)&sand_prop_base_list[43]},	4,       	(u8*)&storage.vars.autosave_period,	"autosave_period",	"Autosave period (sec)",                   	VAR_TYPE_U16,   	1,        	43, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[53],	(void*)&sand_prop_base_list[44]},	2,       	(u8*)&storage.vars.last_save_time, 	"last_save_time", 	"Time of last save",                       	VAR_TYPE_U32,   	1,        	44, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[54],	(void*)&sand_prop_base_list[45]},	2,       	(u8*)&storage.vars.dump_addr,      	"dump_addr",      	"Save dump FLASH address",                 	VAR_TYPE_U32,   	1,        	45, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[55],	(void*)&sand_prop_base_list[46]},	2,       	(u8*)&storage.vars.dump_size,      	"dump_size",      	"Save dump size (bytes)",                  	VAR_TYPE_U16,   	1,        	46, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[56],	(void*)&sand_prop_base_list[47]},	2,       	(u8*)&storage.vars.data_changed,   	"data_changed",   	"Data changed flag",                       	VAR_TYPE_U16,   	1,        	47, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[59],	(void*)&sand_prop_base_list[48]},	4,       	(u8*)&modbus.vars.addr,            	"addr",           	"Device ModBUS address",                   	VAR_TYPE_U16,   	1,        	48, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[60],	(void*)&sand_prop_base_list[49]},	4,       	(u8*)&modbus.vars.addr_err_ingore, 	"addr_err_ingore",	"Ignore ILLEGAL DATA ADDRESS error",       	VAR_TYPE_U16,   	1,        	49, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[61],	(void*)&sand_prop_base_list[50]},	2,       	(u8*)&modbus.vars.rx_cnt,          	"rx_cnt",         	"Received packet counter",                 	VAR_TYPE_U16,   	1,        	50, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[62],	(void*)&sand_prop_base_list[51]},	2,       	(u8*)&modbus.vars.tx_cnt,          	"tx_cnt",         	"Transmitted packet counter",              	VAR_TYPE_U16,   	1,        	51, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[63],	(void*)&sand_prop_base_list[52]},	2,       	(u8*)&modbus.vars.err_cnt,         	"err_cnt",        	"Error packet counter",                    	VAR_TYPE_U16,   	1,        	52, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[64],	(void*)&sand_prop_base_list[53]},	2,       	(u8*)&ai.vars.ai_value,            	"ai_value",       	"Measured value",                          	VAR_TYPE_FLOAT, 	2,        	53, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[65],	(void*)&sand_prop_base_list[54]},	4,       	(u8*)&ai.vars.ai_calib_a,          	"ai_calib_a",     	"Calibration coef A",                      	VAR_TYPE_FLOAT, 	2,        	54, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[66],	(void*)&sand_prop_base_list[55]},	4,       	(u8*)&ai.vars.ai_calib_b,          	"ai_calib_b",     	"Calibration coef B",                      	VAR_TYPE_FLOAT, 	2,        	55, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[67],	(void*)&sand_prop_base_list[56]},	2,       	(u8*)&ai.vars.ai_adc,              	"ai_adc",         	"ADC raw data",                            	VAR_TYPE_U16,   	2,        	56, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[68],	(void*)&sand_prop_base_list[57]},	2,       	(u8*)&ai.vars.ai_sample_rate,      	"ai_sample_rate", 	"Samples per second",                      	VAR_TYPE_FLOAT, 	2,        	57, 	1},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[9], 	(void*)&sand_prop_base_list[58]},	2,       	(u8*)&test.vars.arr_u8,            	"arr_u8",         	"U8 array",                                	VAR_TYPE_U8,    	20,       	58, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[12],	(void*)&sand_prop_base_list[59]},	2,       	(u8*)&test.vars.arr_u16,           	"arr_u16",        	"U16 array",                               	VAR_TYPE_U16,   	20,       	59, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[13],	(void*)&sand_prop_base_list[60]},	2,       	(u8*)&test.vars.arr_u32,           	"arr_u32",        	"U32 array",                               	VAR_TYPE_U32,   	20,       	60, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[39],	(void*)&sand_prop_base_list[61]},	2,       	(u8*)&test.vars.arr_u64,           	"arr_u64",        	"U64 array",                               	VAR_TYPE_U64,   	20,       	61, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[10],	(void*)&sand_prop_base_list[62]},	2,       	(u8*)&test.vars.arr_s8,            	"arr_s8",         	"S8 array",                                	VAR_TYPE_S8,    	20,       	62, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[14],	(void*)&sand_prop_base_list[63]},	2,       	(u8*)&test.vars.arr_s16,           	"arr_s16",        	"S16 array",                               	VAR_TYPE_S16,   	20,       	63, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[15],	(void*)&sand_prop_base_list[64]},	2,       	(u8*)&test.vars.arr_s32,           	"arr_s32",        	"S32 array",                               	VAR_TYPE_S32,   	20,       	64, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[49],	(void*)&sand_prop_base_list[65]},	2,       	(u8*)&test.vars.arr_s64,           	"arr_s64",        	"S64 array",                               	VAR_TYPE_S64,   	20,       	65, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[57],	(void*)&sand_prop_base_list[66]},	2,       	(u8*)&test.vars.arr_float,         	"arr_float",      	"FLOAT array",                             	VAR_TYPE_FLOAT, 	20,       	66, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[58],	(void*)&sand_prop_base_list[67]},	2,       	(u8*)&test.vars.arr_double,        	"arr_double",     	"DOUBLE array",                            	VAR_TYPE_DOUBLE,	20,       	67, 	0},
{{SAND_PROP_BASE,	(void*)&sand_prop_mdb_list[17],	(void*)&sand_prop_base_list[68]},	2,       	(u8*)&test.vars.arr_char,          	"arr_char",       	"CHAR array",                              	VAR_TYPE_CHAR,  	30,       	68, 	0},
};

const sand_prop_mdb_t sand_prop_mdb_list[SAND_PROP_MDB_REG_NUM]={
//prop          	header_next                      	header_base                     	mdb_addr 	   reg_name
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[0]}, 	0},         // os_version
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[1]}, 	4},         // num_of_vars
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[0], 	(void*)&sand_prop_base_list[2]}, 	5},         // lang
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[1],  	(void*)&sand_prop_base_list[3]}, 	6},         // command
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[2],  	(void*)&sand_prop_base_list[4]}, 	7},         // reset_num
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[5]}, 	8},         // runtime
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[6]}, 	10},        // runtime_total
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[7]}, 	12},        // cpu_load
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[8]}, 	20},        // build
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[58]},	40},        // arr_u8
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[62]},	50},        // arr_s8
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[9]}, 	60},        // build_date
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[59]},	70},        // arr_u16
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[60]},	90},        // arr_u32
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[63]},	130},       // arr_s16
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[64]},	150},       // arr_s32
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[10]},	200},       // device_name
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[68]},	220},       // arr_char
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[3],  	(void*)&sand_prop_base_list[11]},	240},       // user_note
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[4],  	(void*)&sand_prop_base_list[12]},	280},       // configuration
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[13]},	320},       // mcu_info
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[14]},	360},       // mcu_id
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[15]},	366},       // device_type
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[5],  	(void*)&sand_prop_base_list[16]},	367},       // board_ver
{{SAND_PROP_MDB,	(void*)&sand_prop_save_list[6],  	(void*)&sand_prop_base_list[17]},	368},       // serial
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[18]},	370},       // temperature
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[19]},	372},       // v_bat
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[20]},	374},       // v_pwr
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[21]},	376},       // vref_int
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[22]},	378},       // vref_ext
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[1], 	(void*)&sand_prop_base_list[23]},	380},       // tmpr_add
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[2], 	(void*)&sand_prop_base_list[24]},	382},       // v_pwr_mul
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[3], 	(void*)&sand_prop_base_list[25]},	384},       // v_pwr_add
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[4], 	(void*)&sand_prop_base_list[26]},	386},       // vref_sel
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[27]},	400},       // rtc_state
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[28]},	402},       // adc_int_state
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[29]},	404},       // rs485_state
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[30]},	406},       // dbg_state
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[31]},	408},       // ai_state
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[61]},	410},       // arr_u64
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[32]},	500},       // year
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[5], 	(void*)&sand_prop_base_list[33]},	501},       // month
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[6], 	(void*)&sand_prop_base_list[34]},	502},       // day
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[7], 	(void*)&sand_prop_base_list[35]},	503},       // hour
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[8], 	(void*)&sand_prop_base_list[36]},	504},       // minute
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[9], 	(void*)&sand_prop_base_list[37]},	505},       // sec
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[10],	(void*)&sand_prop_base_list[38]},	506},       // msec
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[11],	(void*)&sand_prop_base_list[39]},	507},       // utc
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[40]},	508},       // unix
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[65]},	510},       // arr_s64
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[41]},	600},       // erase_cnt
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[12],	(void*)&sand_prop_base_list[42]},	602},       // autosave_en
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[13],	(void*)&sand_prop_base_list[43]},	603},       // autosave_period
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[44]},	604},       // last_save_time
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[45]},	606},       // dump_addr
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[46]},	608},       // dump_size
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[47]},	609},       // data_changed
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[66]},	610},       // arr_float
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[67]},	650},       // arr_double
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[14],	(void*)&sand_prop_base_list[48]},	800},       // addr
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[15],	(void*)&sand_prop_base_list[49]},	801},       // addr_err_ingore
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[50]},	802},       // rx_cnt
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[51]},	803},       // tx_cnt
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[52]},	804},       // err_cnt
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[53]},	1000},      // ai_value
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[16],	(void*)&sand_prop_base_list[54]},	1004},      // ai_calib_a
{{SAND_PROP_MDB,	(void*)&sand_prop_range_list[17],	(void*)&sand_prop_base_list[55]},	1008},      // ai_calib_b
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[56]},	1012},      // ai_adc
{{SAND_PROP_MDB,	NULL,                            	(void*)&sand_prop_base_list[57]},	1014},      // ai_sample_rate
};

const sand_prop_range_t sand_prop_range_list[SAND_PROP_RANGE_REG_NUM]={
//prop            	header_next                     	header_base                     	p_def                     	p_min           	p_max            	   reg_name
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[0], 	(void*)&sand_prop_base_list[2]}, 	(void*)&u16_0,            	(void*)&u16_0,  	(void*)&u16_1},     // lang
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[7], 	(void*)&sand_prop_base_list[23]},	(void*)&float_0_0f,       	NULL,           	NULL},              // tmpr_add
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[8], 	(void*)&sand_prop_base_list[24]},	(void*)&float_16_0f,      	NULL,           	NULL},              // v_pwr_mul
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[9], 	(void*)&sand_prop_base_list[25]},	(void*)&float_0_0f,       	NULL,           	NULL},              // v_pwr_add
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[10],	(void*)&sand_prop_base_list[26]},	(void*)&u16_0,            	(void*)&u16_0,  	(void*)&u16_1},     // vref_sel
{{SAND_PROP_RANGE,	NULL,                           	(void*)&sand_prop_base_list[33]},	NULL,                     	(void*)&u16_1,  	(void*)&u16_12},    // month
{{SAND_PROP_RANGE,	NULL,                           	(void*)&sand_prop_base_list[34]},	NULL,                     	(void*)&u16_1,  	(void*)&u16_31},    // day
{{SAND_PROP_RANGE,	NULL,                           	(void*)&sand_prop_base_list[35]},	NULL,                     	(void*)&u16_0,  	(void*)&u16_23},    // hour
{{SAND_PROP_RANGE,	NULL,                           	(void*)&sand_prop_base_list[36]},	NULL,                     	(void*)&u16_0,  	(void*)&u16_59},    // minute
{{SAND_PROP_RANGE,	NULL,                           	(void*)&sand_prop_base_list[37]},	NULL,                     	(void*)&u16_0,  	(void*)&u16_59},    // sec
{{SAND_PROP_RANGE,	NULL,                           	(void*)&sand_prop_base_list[38]},	NULL,                     	(void*)&u16_0,  	(void*)&u16_999},   // msec
{{SAND_PROP_RANGE,	NULL,                           	(void*)&sand_prop_base_list[39]},	NULL,                     	(void*)&s16__12,	(void*)&s16_14},    // utc
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[11],	(void*)&sand_prop_base_list[42]},	(void*)&u16_1,            	(void*)&u16_0,  	(void*)&u16_1},     // autosave_en
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[12],	(void*)&sand_prop_base_list[43]},	(void*)&u16_10,           	(void*)&u16_1,  	(void*)&u16_1440},  // autosave_period
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[13],	(void*)&sand_prop_base_list[48]},	NULL,                     	NULL,           	(void*)&u16_255},   // addr
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[14],	(void*)&sand_prop_base_list[49]},	NULL,                     	NULL,           	(void*)&u16_1},     // addr_err_ingore
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[15],	(void*)&sand_prop_base_list[54]},	(void*)&float_16_0f_16_0f,	NULL,           	NULL},              // ai_calib_a
{{SAND_PROP_RANGE,	(void*)&sand_prop_save_list[16],	(void*)&sand_prop_base_list[55]},	(void*)&float_0_0f_0_0f,  	NULL,           	NULL},              // ai_calib_b
};

const sand_prop_save_t sand_prop_save_list[SAND_PROP_SAVE_REG_NUM]={
//prop           	header_next 	header_base                     	save_addr 	busy 	   reg_name
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[2]}, 	0,        	0},     // lang
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[3]}, 	2,        	0},     // command
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[4]}, 	4,        	0},     // reset_num
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[11]},	6,        	0},     // user_note
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[12]},	46,       	0},     // configuration
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[16]},	86,       	0},     // board_ver
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[17]},	88,       	0},     // serial
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[23]},	92,       	0},     // tmpr_add
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[24]},	96,       	0},     // v_pwr_mul
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[25]},	100,      	0},     // v_pwr_add
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[26]},	104,      	0},     // vref_sel
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[42]},	106,      	0},     // autosave_en
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[43]},	108,      	0},     // autosave_period
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[48]},	110,      	0},     // addr
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[49]},	112,      	0},     // addr_err_ingore
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[54]},	114,      	0},     // ai_calib_a
{{SAND_PROP_SAVE,	NULL,       	(void*)&sand_prop_base_list[55]},	122,      	0},     // ai_calib_b
};

const sand_prop_access_t sand_prop_access_list[SAND_PROP_ACCESS_REG_NUM]={
//prop 	header_next 	header_base 	access_lvl 	access_en_timer_ms 	   reg_name
};

//#generator_message{"msg":"sand_properties", "action":"insert_end"}
