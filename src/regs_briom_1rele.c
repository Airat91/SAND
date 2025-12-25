/*
 * File:        regs_briom_1rele.c
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Global structs of BRIOM module
 * Revision history: 0.1
 */

#if(DEVICE_TYPE == BRIOM_1RELE)

#include "regs_briom_1rele.h"

//#generator_message{"msg":"sand_properties", "action":"insert_start"}
// This part of file generated automatically, don't change it

//Global structs declaration
os_struct      	os      	 = {0};
device_struct  	device  	 = {0};
service_struct 	service 	 = {0};
time_struct    	time    	 = {0};
storage_struct 	storage 	 = {0};
modbus_struct  	modbus  	 = {0};
rele_struct    	rele    	 = {0};

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
//prop            	header_next                     	header_base                      	prop_num  	p_value                             	name                	description                          	type            	array_len  	ind  	read_only  	
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[0],  	(void*)&sand_prop_base_list[0]},  	2,        	(u8*)&os.vars.os_version,           	"os_version",       	"Software version",                  	VAR_TYPE_U16,   	4,         	0,   	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[1],  	(void*)&sand_prop_base_list[1]},  	2,        	(u8*)&os.vars.num_of_vars,          	"num_of_vars",      	"Total registers in project",        	VAR_TYPE_U16,   	1,         	1,   	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[2],  	(void*)&sand_prop_base_list[2]},  	4,        	(u8*)&os.vars.lang,                 	"lang",             	"Interface language",                	VAR_TYPE_U16,   	1,         	2,   	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[3],  	(void*)&sand_prop_base_list[3]},  	3,        	(u8*)&os.vars.command,              	"command",          	"Command register",                  	VAR_TYPE_U16,   	1,         	3,   	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[4],  	(void*)&sand_prop_base_list[4]},  	3,        	(u8*)&os.vars.reset_num,            	"reset_num",        	"Number of resets",                  	VAR_TYPE_U16,   	1,         	4,   	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[5],  	(void*)&sand_prop_base_list[5]},  	2,        	(u8*)&os.vars.reset_reason,         	"reset_reason",     	"Reason of last reset",              	VAR_TYPE_U16,   	1,         	5,   	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[6],  	(void*)&sand_prop_base_list[6]},  	2,        	(u8*)&os.vars.release,              	"release",          	"Release flag",                      	VAR_TYPE_U16,   	1,         	6,   	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[7],  	(void*)&sand_prop_base_list[7]},  	2,        	(u8*)&os.vars.runtime,              	"runtime",          	"Runtime from last reset (sec)",     	VAR_TYPE_U32,   	1,         	7,   	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[8],  	(void*)&sand_prop_base_list[8]},  	3,        	(u8*)&os.vars.runtime_storage,      	"runtime_storage",  	"Runtime before last reset (sec)",   	VAR_TYPE_U32,   	1,         	8,   	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[9],  	(void*)&sand_prop_base_list[9]},  	2,        	(u8*)&os.vars.runtime_total,        	"runtime_total",    	"Total runtime (sec)",               	VAR_TYPE_U32,   	1,         	9,   	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[10], 	(void*)&sand_prop_base_list[10]}, 	2,        	(u8*)&os.vars.cpu_load,             	"cpu_load",         	"CPU load (%)",                      	VAR_TYPE_FLOAT, 	1,         	10,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[11], 	(void*)&sand_prop_base_list[11]}, 	2,        	(u8*)&os.vars.build,                	"build",            	"Build info",                        	VAR_TYPE_CHAR,  	40,        	11,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[12], 	(void*)&sand_prop_base_list[12]}, 	2,        	(u8*)&os.vars.build_date,           	"build_date",       	"Date of SW building",               	VAR_TYPE_CHAR,  	20,        	12,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[13], 	(void*)&sand_prop_base_list[13]}, 	2,        	(u8*)&device.vars.device_name,      	"device_name",      	"Device name",                       	VAR_TYPE_CHAR,  	40,        	13,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[14], 	(void*)&sand_prop_base_list[14]}, 	3,        	(u8*)&device.vars.user_note,        	"user_note",        	"User device name",                  	VAR_TYPE_CHAR,  	40,        	14,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[15], 	(void*)&sand_prop_base_list[15]}, 	3,        	(u8*)&device.vars.configuration,    	"configuration",    	"Hardware configuration",            	VAR_TYPE_CHAR,  	40,        	15,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[16], 	(void*)&sand_prop_base_list[16]}, 	2,        	(u8*)&device.vars.mcu_info,         	"mcu_info",         	"MCU information",                   	VAR_TYPE_CHAR,  	40,        	16,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[17], 	(void*)&sand_prop_base_list[17]}, 	2,        	(u8*)&device.vars.mcu_id,           	"mcu_id",           	"Unique MCU ID number",              	VAR_TYPE_U8,    	12,        	17,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[18], 	(void*)&sand_prop_base_list[18]}, 	2,        	(u8*)&device.vars.device_type,      	"device_type",      	"Device type code",                  	VAR_TYPE_U16,   	1,         	18,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[19], 	(void*)&sand_prop_base_list[19]}, 	3,        	(u8*)&device.vars.board_ver,        	"board_ver",        	"Hardware version",                  	VAR_TYPE_U16,   	1,         	19,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[20], 	(void*)&sand_prop_base_list[20]}, 	3,        	(u8*)&device.vars.serial,           	"serial",           	"Serial number",                     	VAR_TYPE_U32,   	1,         	20,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[21], 	(void*)&sand_prop_base_list[21]}, 	2,        	(u8*)&device.vars.temperature,      	"temperature",      	"Temperature (°C)",                  	VAR_TYPE_FLOAT, 	1,         	21,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[22], 	(void*)&sand_prop_base_list[22]}, 	2,        	(u8*)&device.vars.v_bat,            	"v_bat",            	"Battery voltage (V)",               	VAR_TYPE_FLOAT, 	1,         	22,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[23], 	(void*)&sand_prop_base_list[23]}, 	2,        	(u8*)&device.vars.v_pwr,            	"v_pwr",            	"Power voltage (V)",                 	VAR_TYPE_FLOAT, 	1,         	23,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[24], 	(void*)&sand_prop_base_list[24]}, 	2,        	(u8*)&device.vars.vref_int,         	"vref_int",         	"Vref internal (V)",                 	VAR_TYPE_FLOAT, 	1,         	24,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[25], 	(void*)&sand_prop_base_list[25]}, 	2,        	(u8*)&device.vars.vref_ext,         	"vref_ext",         	"Vref external (V)",                 	VAR_TYPE_FLOAT, 	1,         	25,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[26], 	(void*)&sand_prop_base_list[26]}, 	4,        	(u8*)&device.vars.tmpr_add,         	"tmpr_add",         	"Temperature add coeff",             	VAR_TYPE_FLOAT, 	1,         	26,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[27], 	(void*)&sand_prop_base_list[27]}, 	4,        	(u8*)&device.vars.v_pwr_mul,        	"v_pwr_mul",        	"Power voltage mul coeff",           	VAR_TYPE_FLOAT, 	1,         	27,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[28], 	(void*)&sand_prop_base_list[28]}, 	4,        	(u8*)&device.vars.v_pwr_add,        	"v_pwr_add",        	"Power voltage add coeff",           	VAR_TYPE_FLOAT, 	1,         	28,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[29], 	(void*)&sand_prop_base_list[29]}, 	4,        	(u8*)&device.vars.vref_sel,         	"vref_sel",         	"Select Vref source (0-Int, 1-Ext)", 	VAR_TYPE_U16,   	1,         	29,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[30], 	(void*)&sand_prop_base_list[30]}, 	2,        	(u8*)&service.vars.rtc_state,       	"rtc_state",        	"RTC service status",                	VAR_TYPE_U32,   	1,         	30,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[31], 	(void*)&sand_prop_base_list[31]}, 	2,        	(u8*)&service.vars.adc_int_state,   	"adc_int_state",    	"ADC_INT service status",            	VAR_TYPE_U32,   	1,         	31,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[32], 	(void*)&sand_prop_base_list[32]}, 	2,        	(u8*)&service.vars.rs485_state,     	"rs485_state",      	"RS-485 service status",             	VAR_TYPE_U32,   	1,         	32,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[33], 	(void*)&sand_prop_base_list[33]}, 	2,        	(u8*)&service.vars.dbg_state,       	"dbg_state",        	"Debug service status",              	VAR_TYPE_U32,   	1,         	33,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[34], 	(void*)&sand_prop_base_list[34]}, 	2,        	(u8*)&service.vars.ai_state,        	"ai_state",         	"AI service status",                 	VAR_TYPE_U32,   	1,         	34,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[35], 	(void*)&sand_prop_base_list[35]}, 	2,        	(u8*)&time.vars.year,               	"year",             	"Year",                              	VAR_TYPE_U16,   	1,         	35,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[36], 	(void*)&sand_prop_base_list[36]}, 	3,        	(u8*)&time.vars.month,              	"month",            	"Month [1..12]",                     	VAR_TYPE_U16,   	1,         	36,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[37], 	(void*)&sand_prop_base_list[37]}, 	3,        	(u8*)&time.vars.day,                	"day",              	"Day [1..31]",                       	VAR_TYPE_U16,   	1,         	37,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[38], 	(void*)&sand_prop_base_list[38]}, 	3,        	(u8*)&time.vars.hour,               	"hour",             	"Hour [0..23]",                      	VAR_TYPE_U16,   	1,         	38,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[39], 	(void*)&sand_prop_base_list[39]}, 	3,        	(u8*)&time.vars.minute,             	"minute",           	"Minute [0..59]",                    	VAR_TYPE_U16,   	1,         	39,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[40], 	(void*)&sand_prop_base_list[40]}, 	3,        	(u8*)&time.vars.sec,                	"sec",              	"Sec [0..59]",                       	VAR_TYPE_U16,   	1,         	40,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[41], 	(void*)&sand_prop_base_list[41]}, 	3,        	(u8*)&time.vars.msec,               	"msec",             	"Msec [0..999]",                     	VAR_TYPE_U16,   	1,         	41,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[42], 	(void*)&sand_prop_base_list[42]}, 	3,        	(u8*)&time.vars.utc,                	"utc",              	"Utc [-12..14]",                     	VAR_TYPE_S16,   	1,         	42,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[43], 	(void*)&sand_prop_base_list[43]}, 	2,        	(u8*)&time.vars.unix,               	"unix",             	"Unix timestamp",                    	VAR_TYPE_U32,   	1,         	43,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[44], 	(void*)&sand_prop_base_list[44]}, 	2,        	(u8*)&storage.vars.erase_cnt,       	"erase_cnt",        	"Storage FLASH erase counter",       	VAR_TYPE_U32,   	1,         	44,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[45], 	(void*)&sand_prop_base_list[45]}, 	4,        	(u8*)&storage.vars.autosave_en,     	"autosave_en",      	"Enable autosave",                   	VAR_TYPE_U16,   	1,         	45,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[46], 	(void*)&sand_prop_base_list[46]}, 	4,        	(u8*)&storage.vars.autosave_period, 	"autosave_period",  	"Autosave period (sec)",             	VAR_TYPE_U16,   	1,         	46,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[47], 	(void*)&sand_prop_base_list[47]}, 	2,        	(u8*)&storage.vars.last_save_time,  	"last_save_time",   	"Time of last save",                 	VAR_TYPE_U32,   	1,         	47,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[48], 	(void*)&sand_prop_base_list[48]}, 	2,        	(u8*)&storage.vars.dump_addr,       	"dump_addr",        	"Save dump FLASH address",           	VAR_TYPE_U32,   	1,         	48,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[49], 	(void*)&sand_prop_base_list[49]}, 	2,        	(u8*)&storage.vars.dump_position,   	"dump_position",    	"Save dump FLASH position",          	VAR_TYPE_U16,   	1,         	49,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[50], 	(void*)&sand_prop_base_list[50]}, 	2,        	(u8*)&storage.vars.dump_size,       	"dump_size",        	"Save dump size (bytes)",            	VAR_TYPE_U16,   	1,         	50,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[51], 	(void*)&sand_prop_base_list[51]}, 	2,        	(u8*)&storage.vars.storage_size,    	"storage_size",     	"Storage FLASH size (bytes)",        	VAR_TYPE_U16,   	1,         	51,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[52], 	(void*)&sand_prop_base_list[52]}, 	2,        	(u8*)&storage.vars.data_changed,    	"data_changed",     	"Data changed flag",                 	VAR_TYPE_U16,   	1,         	52,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[53], 	(void*)&sand_prop_base_list[53]}, 	2,        	(u8*)&storage.vars.duration_us,     	"duration_us",      	"Duration of save operation (us)",   	VAR_TYPE_U16,   	1,         	53,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[54], 	(void*)&sand_prop_base_list[54]}, 	4,        	(u8*)&modbus.vars.addr,             	"addr",             	"Device ModBUS address",             	VAR_TYPE_U16,   	1,         	54,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[55], 	(void*)&sand_prop_base_list[55]}, 	4,        	(u8*)&modbus.vars.addr_err_ingore,  	"addr_err_ingore",  	"Ignore ILLEGAL DATA ADDRESS error", 	VAR_TYPE_U16,   	1,         	55,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[56], 	(void*)&sand_prop_base_list[56]}, 	2,        	(u8*)&modbus.vars.rx_cnt,           	"rx_cnt",           	"Received packet counter",           	VAR_TYPE_U16,   	1,         	56,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[57], 	(void*)&sand_prop_base_list[57]}, 	2,        	(u8*)&modbus.vars.tx_cnt,           	"tx_cnt",           	"Transmitted packet counter",        	VAR_TYPE_U16,   	1,         	57,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[58], 	(void*)&sand_prop_base_list[58]}, 	2,        	(u8*)&modbus.vars.err_cnt,          	"err_cnt",          	"Error packet counter",              	VAR_TYPE_U16,   	1,         	58,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[59], 	(void*)&sand_prop_base_list[59]}, 	2,        	(u8*)&rele.vars.ai_value,           	"ai_value",         	"Measured value",                    	VAR_TYPE_FLOAT, 	2,         	59,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[60], 	(void*)&sand_prop_base_list[60]}, 	4,        	(u8*)&rele.vars.ai_calib_a,         	"ai_calib_a",       	"Calibration coef A",                	VAR_TYPE_FLOAT, 	2,         	60,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[61], 	(void*)&sand_prop_base_list[61]}, 	4,        	(u8*)&rele.vars.ai_calib_b,         	"ai_calib_b",       	"Calibration coef B",                	VAR_TYPE_FLOAT, 	2,         	61,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[62], 	(void*)&sand_prop_base_list[62]}, 	2,        	(u8*)&rele.vars.rele_version,       	"rele_version",     	"Software version",                  	VAR_TYPE_U16,   	2,         	62,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[63], 	(void*)&sand_prop_base_list[63]}, 	2,        	(u8*)&rele.vars.rele_ctrl,          	"rele_ctrl",        	"Control rele state (0-off, 1-on)",  	VAR_TYPE_U16,   	1,         	63,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[64], 	(void*)&sand_prop_base_list[64]}, 	2,        	(u8*)&rele.vars.rele_cnt,           	"rele_cnt",         	"Rele counter from last reset",      	VAR_TYPE_U32,   	1,         	64,  	1},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[65], 	(void*)&sand_prop_base_list[65]}, 	3,        	(u8*)&rele.vars.rele_cnt_storage,   	"rele_cnt_storage", 	"Rele counter before last reset",    	VAR_TYPE_U32,   	1,         	65,  	0},
{{SAND_PROP_BASE, 	(void*)&sand_prop_mdb_list[66], 	(void*)&sand_prop_base_list[66]}, 	2,        	(u8*)&rele.vars.rele_cnt_total,     	"rele_cnt_total",   	"Rele counter total",                	VAR_TYPE_U32,   	1,         	66,  	1},
};

const sand_prop_mdb_t sand_prop_mdb_list[SAND_PROP_MDB_REG_NUM]={
//prop           	header_next                         	header_base                      	mdb_addr  	   reg_name
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[0]},  	0},         // os_version
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[1]},  	4},         // num_of_vars
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[0],    	(void*)&sand_prop_base_list[2]},  	5},         // lang
{{SAND_PROP_MDB, 	(void*)&sand_prop_callback_list[0], 	(void*)&sand_prop_base_list[3]},  	6},         // command
{{SAND_PROP_MDB, 	(void*)&sand_prop_save_list[1],     	(void*)&sand_prop_base_list[4]},  	7},         // reset_num
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[5]},  	8},         // reset_reason
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[6]},  	9},         // release
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[7]},  	10},        // runtime
{{SAND_PROP_MDB, 	(void*)&sand_prop_save_list[2],     	(void*)&sand_prop_base_list[8]},  	12},        // runtime_storage
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[9]},  	14},        // runtime_total
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[10]}, 	16},        // cpu_load
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[11]}, 	20},        // build
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[12]}, 	60},        // build_date
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[13]}, 	200},       // device_name
{{SAND_PROP_MDB, 	(void*)&sand_prop_save_list[3],     	(void*)&sand_prop_base_list[14]}, 	240},       // user_note
{{SAND_PROP_MDB, 	(void*)&sand_prop_save_list[4],     	(void*)&sand_prop_base_list[15]}, 	280},       // configuration
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[16]}, 	320},       // mcu_info
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[17]}, 	360},       // mcu_id
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[18]}, 	366},       // device_type
{{SAND_PROP_MDB, 	(void*)&sand_prop_save_list[5],     	(void*)&sand_prop_base_list[19]}, 	367},       // board_ver
{{SAND_PROP_MDB, 	(void*)&sand_prop_save_list[6],     	(void*)&sand_prop_base_list[20]}, 	368},       // serial
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[21]}, 	370},       // temperature
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[22]}, 	372},       // v_bat
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[23]}, 	374},       // v_pwr
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[24]}, 	376},       // vref_int
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[25]}, 	378},       // vref_ext
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[1],    	(void*)&sand_prop_base_list[26]}, 	380},       // tmpr_add
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[2],    	(void*)&sand_prop_base_list[27]}, 	382},       // v_pwr_mul
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[3],    	(void*)&sand_prop_base_list[28]}, 	384},       // v_pwr_add
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[4],    	(void*)&sand_prop_base_list[29]}, 	386},       // vref_sel
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[30]}, 	400},       // rtc_state
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[31]}, 	402},       // adc_int_state
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[32]}, 	404},       // rs485_state
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[33]}, 	406},       // dbg_state
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[34]}, 	408},       // ai_state
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[35]}, 	500},       // year
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[5],    	(void*)&sand_prop_base_list[36]}, 	501},       // month
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[6],    	(void*)&sand_prop_base_list[37]}, 	502},       // day
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[7],    	(void*)&sand_prop_base_list[38]}, 	503},       // hour
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[8],    	(void*)&sand_prop_base_list[39]}, 	504},       // minute
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[9],    	(void*)&sand_prop_base_list[40]}, 	505},       // sec
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[10],   	(void*)&sand_prop_base_list[41]}, 	506},       // msec
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[11],   	(void*)&sand_prop_base_list[42]}, 	507},       // utc
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[43]}, 	508},       // unix
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[44]}, 	600},       // erase_cnt
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[12],   	(void*)&sand_prop_base_list[45]}, 	602},       // autosave_en
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[13],   	(void*)&sand_prop_base_list[46]}, 	603},       // autosave_period
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[47]}, 	604},       // last_save_time
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[48]}, 	606},       // dump_addr
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[49]}, 	608},       // dump_position
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[50]}, 	609},       // dump_size
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[51]}, 	610},       // storage_size
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[52]}, 	611},       // data_changed
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[53]}, 	612},       // duration_us
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[14],   	(void*)&sand_prop_base_list[54]}, 	800},       // addr
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[15],   	(void*)&sand_prop_base_list[55]}, 	801},       // addr_err_ingore
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[56]}, 	802},       // rx_cnt
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[57]}, 	803},       // tx_cnt
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[58]}, 	804},       // err_cnt
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[59]}, 	1000},      // ai_value
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[16],   	(void*)&sand_prop_base_list[60]}, 	1004},      // ai_calib_a
{{SAND_PROP_MDB, 	(void*)&sand_prop_range_list[17],   	(void*)&sand_prop_base_list[61]}, 	1008},      // ai_calib_b
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[62]}, 	1110},      // rele_version
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[63]}, 	1112},      // rele_ctrl
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[64]}, 	1113},      // rele_cnt
{{SAND_PROP_MDB, 	(void*)&sand_prop_save_list[17],    	(void*)&sand_prop_base_list[65]}, 	1115},      // rele_cnt_storage
{{SAND_PROP_MDB, 	NULL,                               	(void*)&sand_prop_base_list[66]}, 	1117},      // rele_cnt_total
};

const sand_prop_range_t sand_prop_range_list[SAND_PROP_RANGE_REG_NUM]={
//prop             	header_next                      	header_base                      	p_def                      	p_min            	p_max             	   reg_name
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[0],  	(void*)&sand_prop_base_list[2]},  	(void*)&u16_0,             	(void*)&u16_0,   	(void*)&u16_1},     // lang
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[7],  	(void*)&sand_prop_base_list[26]}, 	(void*)&float_0_0f,        	NULL,            	NULL},              // tmpr_add
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[8],  	(void*)&sand_prop_base_list[27]}, 	(void*)&float_16_0f,       	NULL,            	NULL},              // v_pwr_mul
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[9],  	(void*)&sand_prop_base_list[28]}, 	(void*)&float_0_0f,        	NULL,            	NULL},              // v_pwr_add
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[10], 	(void*)&sand_prop_base_list[29]}, 	(void*)&u16_1,             	(void*)&u16_0,   	(void*)&u16_1},     // vref_sel
{{SAND_PROP_RANGE, 	NULL,                            	(void*)&sand_prop_base_list[36]}, 	NULL,                      	(void*)&u16_1,   	(void*)&u16_12},    // month
{{SAND_PROP_RANGE, 	NULL,                            	(void*)&sand_prop_base_list[37]}, 	NULL,                      	(void*)&u16_1,   	(void*)&u16_31},    // day
{{SAND_PROP_RANGE, 	NULL,                            	(void*)&sand_prop_base_list[38]}, 	NULL,                      	(void*)&u16_0,   	(void*)&u16_23},    // hour
{{SAND_PROP_RANGE, 	NULL,                            	(void*)&sand_prop_base_list[39]}, 	NULL,                      	(void*)&u16_0,   	(void*)&u16_59},    // minute
{{SAND_PROP_RANGE, 	NULL,                            	(void*)&sand_prop_base_list[40]}, 	NULL,                      	(void*)&u16_0,   	(void*)&u16_59},    // sec
{{SAND_PROP_RANGE, 	NULL,                            	(void*)&sand_prop_base_list[41]}, 	NULL,                      	(void*)&u16_0,   	(void*)&u16_999},   // msec
{{SAND_PROP_RANGE, 	NULL,                            	(void*)&sand_prop_base_list[42]}, 	NULL,                      	(void*)&s16__12, 	(void*)&s16_14},    // utc
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[11], 	(void*)&sand_prop_base_list[45]}, 	(void*)&u16_1,             	(void*)&u16_0,   	(void*)&u16_1},     // autosave_en
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[12], 	(void*)&sand_prop_base_list[46]}, 	(void*)&u16_10,            	(void*)&u16_1,   	(void*)&u16_1440},  // autosave_period
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[13], 	(void*)&sand_prop_base_list[54]}, 	NULL,                      	NULL,            	(void*)&u16_255},   // addr
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[14], 	(void*)&sand_prop_base_list[55]}, 	NULL,                      	NULL,            	(void*)&u16_1},     // addr_err_ingore
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[15], 	(void*)&sand_prop_base_list[60]}, 	(void*)&float_16_0f_16_0f, 	NULL,            	NULL},              // ai_calib_a
{{SAND_PROP_RANGE, 	(void*)&sand_prop_save_list[16], 	(void*)&sand_prop_base_list[61]}, 	(void*)&float_0_0f_0_0f,   	NULL,            	NULL},              // ai_calib_b
};

const sand_prop_save_t sand_prop_save_list[SAND_PROP_SAVE_REG_NUM]={
//prop            	header_next  	header_base                      	save_addr  	busy  	   reg_name
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[2]},  	0,         	0},     // lang
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[4]},  	2,         	0},     // reset_num
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[8]},  	4,         	0},     // runtime_storage
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[14]}, 	8,         	0},     // user_note
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[15]}, 	48,        	0},     // configuration
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[19]}, 	88,        	0},     // board_ver
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[20]}, 	90,        	0},     // serial
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[26]}, 	94,        	0},     // tmpr_add
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[27]}, 	98,        	0},     // v_pwr_mul
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[28]}, 	102,       	0},     // v_pwr_add
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[29]}, 	106,       	0},     // vref_sel
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[45]}, 	108,       	0},     // autosave_en
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[46]}, 	110,       	0},     // autosave_period
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[54]}, 	112,       	0},     // addr
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[55]}, 	114,       	0},     // addr_err_ingore
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[60]}, 	116,       	0},     // ai_calib_a
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[61]}, 	124,       	0},     // ai_calib_b
{{SAND_PROP_SAVE, 	NULL,        	(void*)&sand_prop_base_list[65]}, 	132,       	0},     // rele_cnt_storage
};

const sand_prop_access_t sand_prop_access_list[SAND_PROP_ACCESS_REG_NUM]={
//prop  	header_next  	header_base  	access_lvl  	access_en_timer_ms  	   reg_name
};

const sand_prop_callback_t sand_prop_callback_list[SAND_PROP_CALLBACK_REG_NUM]={
//prop                	header_next  	header_base                     	(*callback)(void)  	only_end  	   reg_name
{{SAND_PROP_CALLBACK, 	NULL,        	(void*)&sand_prop_base_list[3]}, 	cmd_sand_callback, 	1},         // command
};

//#generator_message{"msg":"sand_properties", "action":"insert_end"}

#endif // (DEVICE_TYPE == BRIOM_1RELE)
