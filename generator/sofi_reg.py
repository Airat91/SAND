# This file is base for generate sofi_option_t structs
# For adding new sofi_option add it to "sofi_prop_t enumeration and create new struct below

descr_max_len = 40
name_max_len = 20

# Enumeration of available properties
sofi_prop_t = {
	# sofi_prop_XXX_t		# description string
	"sofi_prop_base_t":		"Base properties for variable",
	"sofi_prop_mdb_t":		"ModBUS properties",
	"sofi_prop_range_t":	"Min, Max, and default values",
	"sofi_prop_save_t":		"Properties for save in BKRAM or FLASH",
	"sofi_prop_access_t":	"Access properties",
}

# Properties declaration
sofi_prop_base_t = {
	# opt_name		# opt_type						# comment for header-file
	"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
	"prop_num":		{"type": 	"u16",				"comment":	"Number of available properties for variable"},
	"p_value":		{"type":	"u8*",				"comment":	"Pointer to value"},
	"name":			{"type":	"char*",			"comment":	"Unique register name"},
	"description":	{"type":	"char*",			"comment":	"Description of variable"},
	"type":			{"type":	"u8",				"comment":	"Variable type (see @sofi_var_t enumeration)"},
	"array_len":	{"type":	"u16",				"comment":	"Variable array length (=1 if variable is a single value)"},
	"ind":			{"type":	"u16",				"comment":	"Unique variable index"},
	"read_only":	{"type":	"u8",				"comment":	"If setted, variable can't be changed by external interfaces"},
}
sofi_prop_mdb_t = {
	# opt_name		# opt_type						# comment for header-file
	"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
	"mdb_addr":		{"type": 	"u16",				"comment":	"ModBUS address of register"},
}
sofi_prop_range_t = {
	# opt_name		# opt_type						# comment for header-file
	"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
	"p_def":		{"type": 	"const void *",		"comment":	"Default value"},
	"p_min":		{"type": 	"const void *",		"comment":	"Minimum value"},
	"p_max":		{"type": 	"const void *",		"comment":	"maximum value"},
}
sofi_prop_save_t = {
	# opt_name		# opt_type						# comment for header-file
	"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
	"save_addr":	{"type": 	"u16",				"comment":	"Address like byte number in contiguous save array"},
}
sofi_prop_access_t = {
	# opt_name		# opt_type						# comment for header-file
	"header":		{"type":	"sofi_header_t",	"comment":	"Header of property"},
	"access_lvl":	{"type": 	"u8",				"comment":	"Access level (higher value is strongly protect)"},
	"access_en_timer_ms":	{"type": 	"u32",		"comment":	"Timer for access (if =0 the access disable)"},
}