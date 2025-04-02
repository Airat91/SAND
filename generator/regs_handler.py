import project_generator as generator
import colorama
from colorama import Fore, Back, Style, init
import sofi_reg
import copy
import math

def regs_handler(Proj):
    """
    1. Validate regs in project structs
    2. Appoint regs parameters
    3. Appoint property headers links
    4. Create struct declarations (for regs_module.h)
    5. Create property lists for each sofi_prop_t (for regs_module.c)
    :param Proj: class Project (project_generator.Project)
    :return:
    """
    regs_to_delete = []

    #1. Validate unique regs names and appoint unique index
    reg_name_list = []
    reg_index = 0
    for struct_name in Proj.struct_list:
        struct = Proj.struct_list[struct_name]
        for reg_name in struct["reg_list"]:
            reg = struct["reg_list"][reg_name]
            name = reg["sofi_prop_base_t"]["name"]["value"]
            type = reg["sofi_prop_base_t"]["type"]["value"]
            if type == None:
                Proj.errors["err_msg"].append("register \"{}\" in struct \"{}\" type \"{}\" is undefined".format(name, struct_name, type))
                Proj.errors["err_cnt"] += 1
                reg_to_delete = {"reg": reg_name, "struct": struct_name}
                regs_to_delete.append(reg_to_delete)
            else:
                var_type = "VAR_TYPE_{}".format(type.upper())
                if var_type not in sofi_reg.sofi_var_t:
                    Proj.errors["err_msg"].append("register \"{}\" in struct \"{}\" type \"{}\" is unknown".format(name, struct_name, type))
                    Proj.errors["err_cnt"] += 1
                    reg_to_delete = {"reg": reg_name, "struct": struct_name}
                    regs_to_delete.append(reg_to_delete)
                else:
                    reg["sofi_prop_base_t"]["type"]["value"] = var_type
                    if name not in reg_name_list:
                        reg_name_list.append(name)
                    else:
                        print(Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" already exist, therefore "
                                                           "renamed to \"{}_{}\"".format(name, struct_name, name, reg_index))
                        name = "{}_{}".format(name, reg_index)
                        reg["sofi_prop_base_t"]["name"]["value"] = name
                        reg_name_list.append(name)
                    reg["sofi_prop_base_t"]["ind"]["value"] = reg_index
                    reg_index += 1
    if len(regs_to_delete) > 0:
        for reg in regs_to_delete:
            Proj.struct_list[reg["struct"]]["reg_list"].pop(reg["reg"])


    #2. Appoint parameters
    mdb_addr_list = []      # ModBUS addresses
    save_addr_list = []     # Save addresses
    for struct_name in Proj.struct_list:
        struct = Proj.struct_list[struct_name]
        for reg_name in struct["reg_list"]:
            reg = struct["reg_list"][reg_name]
            reg_byte_len = int(reg["sofi_prop_base_t"]["array_len"]["value"]) * sofi_reg.sofi_var_t[reg["sofi_prop_base_t"]["type"]["value"]]["byte_num"]
            mdb_addr = reg["sofi_prop_mdb_t"]["mdb_addr"]["value"]
            if mdb_addr.isdigit():
                for i in range(0, math.ceil(reg_byte_len/2)):
                    if mdb_addr not in mdb_addr_list:
                        mdb_addr_list.append(int(mdb_addr))
                    else:
                        Proj.errors["err_msg"].append("register \"{}\" in struct \"{}\" ModBUS address conflict".format(reg_name, struct_name))
                        Proj.errors["err_cnt"] += 1
                    mdb_addr += 1
    print("Regs_handler done. Handled {} registers".format(reg_index))