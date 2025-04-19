# This file is base for generate sofi_option_t structs
# For adding new sofi_option add it to "sofi_prop_list" dictionary

SOFI_LIMITS = {
	"descr_max_len" 	: 40,
	"name_max_len" 		: 20,
	"mdb_addr_min" 		: 0,
	"mdb_addr_max" 		: 65535,
	"save_addr_min" 	: 0,
	"save_addr_max" 	: 2047, # 2Kb
	"access_lvl_min"	: 0,
	"access_lvl_max"	: 4,
}

# Available types of variables
sofi_var_t = {
	# var_type_name			# opt_type			# comment for header-file
	"VAR_TYPE_UNKNOWN":		{"byte_num":	0,	"comment":	"Error value"},
    "VAR_TYPE_U8":			{"byte_num":	1,	"comment":	"uint8_t"},
    "VAR_TYPE_U16":			{"byte_num":	2,	"comment":	"uint16_t"},
    "VAR_TYPE_U32":			{"byte_num":	4,	"comment":	"uint32_t"},
    "VAR_TYPE_U64":			{"byte_num":	8,	"comment":	"uint64_t"},
    "VAR_TYPE_S8":			{"byte_num":	1,	"comment":	"int8_t"},
    "VAR_TYPE_S16":			{"byte_num":	2,	"comment":	"int16_t"},
    "VAR_TYPE_S32":			{"byte_num":	4,	"comment":	"int32_t"},
    "VAR_TYPE_S64":			{"byte_num":	8,	"comment":	"int64_t"},
	"VAR_TYPE_FLOAT":		{"byte_num":	4,	"comment":	"float"},
    "VAR_TYPE_DOUBLE":		{"byte_num":	8,	"comment":	"double"},
	"VAR_TYPE_CHAR":		{"byte_num":	1,	"comment":	"char"},
}

#Available types of access level
sofi_access_lvl_t = {
	# lvl_name				# lvl_value		# comment for header-file
	"ACC_LVL_ROOT":			{"value": 	0,	"comment":	"Only for developers"},
	"ACC_LVL_ADMIN_HIGH":	{"value": 	1,	"comment":	"Administrator  with high priority"},
	"ACC_LVL_ADMIN_LOW":	{"value": 	2,	"comment":	"Administrator"},
	"ACC_LVL_USER_HIGH":	{"value": 	3,	"comment":	"User with high priority"},
	"ACC_LVL_USER_LOW":		{"value": 	4,	"comment":	"User"},
}

# Header of sofi_prop_t
sofi_header_t = {
	# opt_name			# opt_type						# comment for header-file
	"prop":				{"type":	"sofi_prop_enum_t",	"comment":	"Type of property"},
	"header_next":		{"type":	"void *",			"comment":	"Pointer to next header"},
	"header_base":		{"type":	"void *",			"comment":	"Pointer to base header"},
}

# Properties declaration
sofi_prop_list = {
	"sofi_prop_base_t": {
		# opt_name		# opt_type						# comment for header-file
		"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
		"prop_num":		{"type": 	"u16",				"comment":	"Number of available properties for variable"},
		"p_value":		{"type":	"u8*",				"comment":	"Pointer to value"},
		"name":			{"type":	"char*",			"comment":	"Unique register name"},
		"description":	{"type":	"char*",			"comment":	"Description of variable"},
		"type":			{"type":	"sofi_var_t",		"comment":	"Variable type (see @sofi_var_t enumeration)"},
		"array_len":	{"type":	"u16",				"comment":	"Variable array length (=1 if variable is a single value)"},
		"ind":			{"type":	"u16",				"comment":	"Unique variable index"},
		"read_only":	{"type":	"u8",				"comment":	"If setted, variable can't be changed by external interfaces"},
	},
	"sofi_prop_mdb_t": 	{
		# opt_name		# opt_type						# comment for header-file
		"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
		"mdb_addr":		{"type": 	"u16",				"comment":	"ModBUS address of register"},
	},
	"sofi_prop_range_t":{
		# opt_name		# opt_type						# comment for header-file
		"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
		"p_def":		{"type": 	"const void *",		"comment":	"Default value"},
		"p_min":		{"type": 	"const void *",		"comment":	"Minimum value"},
		"p_max":		{"type": 	"const void *",		"comment":	"Maximum value"},
	},
	"sofi_prop_save_t" :{
		# opt_name		# opt_type						# comment for header-file
		"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
		"save_addr":	{"type": 	"u16",				"comment":	"Address like byte number in contiguous save array"},
	},
	"sofi_prop_access_t":{
		# opt_name		# opt_type						# comment for header-file
		"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
		"access_lvl":	{"type": 	"u8",				"comment":	"Access level (lower value is strongly protect)"},
		"access_en_timer_ms":	{"type": 	"u32",		"comment":	"Timer for access (if =0 the access disable)"},
	},
}

class reg():
	def __init__(self):
		self.name = ""
		self.type = ""
		self.struct = ""
		self.size_in_bytes = 0
		self.prop_list = {}
		for prop_name in sofi_prop_list:
			self.prop_list[prop_name] = {}
			for prop in sofi_prop_list[prop_name]:
				self.prop_list[prop_name][prop] = {}