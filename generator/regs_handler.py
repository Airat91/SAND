#from Scripts.rst2html4 import description
from csv import excel
from types import NoneType

import project_generator as generator
import colorama
from colorama import Fore, Back, Style, init
import sofi_reg
import copy
import math

def regs_handler(Proj):
    """
    1. Validate regs names and regs types in project structs and appoint indexes
    2. Appoint regs parameters for each prop_list
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
            #1.1 Check reg name
            name = reg["sofi_prop_base_t"]["name"]["value"]
            if len(name) > sofi_reg.SOFI_LIMITS["name_max_len"]:
                shorted_name = name[0:sofi_reg.SOFI_LIMITS["name_max_len"]]
                print(Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" name is too long and shorted "
                                                   "to {}".format(name, struct_name, shorted_name))
                name = shorted_name
            if name not in reg_name_list:
                reg_name_list.append(name)
            else:
                print(Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" already exist, therefore "
                                                   "renamed to \"{}_{}\"".format(name, struct_name, name, reg_index))
                name = "{}_{}".format(name, reg_index)
                reg["sofi_prop_base_t"]["name"]["value"] = name
                reg_name_list.append(name)
            #1.2 Check reg type
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
                    # Add reg to delete list
                    reg_to_delete = {"reg": reg_name, "struct": struct_name}
                    find = False
                    for reg in regs_to_delete:
                        if reg["reg"] == reg_name:
                            find = True
                            break
                    if find == False:
                        regs_to_delete.append(reg_to_delete)
                else:
                    reg["sofi_prop_base_t"]["type"]["value"] = var_type

            if reg not in regs_to_delete:
                reg["sofi_prop_base_t"]["ind"]["value"] = reg_index
                reg_index += 1

    if len(regs_to_delete) > 0:
        for reg in regs_to_delete:
            Proj.struct_list[reg["struct"]]["reg_list"].pop(reg["reg"])

    #2. Appoint regs parameters for each prop_list
    #2.1 sofi_prop_base_t
    for reg_name in Proj.prop_list["sofi_prop_base_t"]["reg_list"]:
        reg = Proj.prop_list["sofi_prop_base_t"]["reg_list"][reg_name]
        struct_name = get_struct_of_reg(reg_name, Proj)
        #2.1.1 Check reg description
        description = reg["sofi_prop_base_t"]["description"]["value"]
        if isinstance(description, str):
            if len(description) > sofi_reg.SOFI_LIMITS["descr_max_len"]:
                shorted_description = description[0:sofi_reg.SOFI_LIMITS["descr_max_len"]]
                print(
                    Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" description is too long and shorted "
                                                 "to {}".format(reg_name, struct_name, shorted_description))
                reg["sofi_prop_base_t"]["description"]["value"] = shorted_description
        #2.1.2 Check array len
        array_len = reg["sofi_prop_base_t"]["array_len"]["value"]
        if isinstance(array_len, int):
            array_len = array_len
        else:
            reg["sofi_prop_base_t"]["array_len"]["value"] = 1
            print(
                Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" array len is undefined so setted "
                                             "to 1".format(reg_name, struct_name))
        #2.1.3 Check read_only flag
        read_only = reg["sofi_prop_base_t"]["read_only"]["value"]
        if read_only == None:
            reg["sofi_prop_base_t"]["read_only"]["value"] = 0
            print(
                Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" read_only is undefined so setted "
                                             "to 0".format(reg_name, struct_name))
        elif (read_only == 1) or (read_only.lower() == "true"):
            reg["sofi_prop_base_t"]["read_only"]["value"] = 1
        elif (read_only == 0) or (read_only.lower() == "false"):
            reg["sofi_prop_base_t"]["read_only"]["value"] = 0
        else:
            reg["sofi_prop_base_t"]["read_only"]["value"] = 0
            print(
                Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" read_only is undefined so setted "
                                             "to 0".format(reg_name, struct_name))

    #2.2 sofi_prop_mdb_t
    mdb_addr_list = []      # ModBUS addresses
    #2.2.1 Manual setted ModBUS address check
    for reg_name in Proj.prop_list["sofi_prop_mdb_t"]["reg_list"]:
        reg = Proj.prop_list["sofi_prop_mdb_t"]["reg_list"][reg_name]
        struct_name = get_struct_of_reg(reg_name, Proj)
        reg_byte_len = int(reg["sofi_prop_base_t"]["array_len"]["value"]) * \
                       sofi_reg.sofi_var_t[reg["sofi_prop_base_t"]["type"]["value"]]["byte_num"]
        # mdb_addr reading
        mdb_addr = reg["sofi_prop_mdb_t"]["mdb_addr"]["value"]
        if isinstance(mdb_addr, int):
            for i in range(0, math.ceil(reg_byte_len/2)):
                if mdb_addr not in mdb_addr_list:
                    mdb_addr_list.append(int(mdb_addr))
                else:
                    Proj.errors["err_msg"].append("register \"{}\" in struct \"{}\" ModBUS address conflict".format(reg_name, struct_name))
                    Proj.errors["err_cnt"] += 1
                    break
                mdb_addr += 1
    #2.2.2 Appoint ModBUS addresses
    mdb_ind = 0
    for reg_name in Proj.prop_list["sofi_prop_mdb_t"]["reg_list"]:
        reg = Proj.prop_list["sofi_prop_mdb_t"]["reg_list"][reg_name]
        struct_name = get_struct_of_reg(reg_name, Proj)
        reg_byte_len = int(reg["sofi_prop_base_t"]["array_len"]["value"]) * \
                       sofi_reg.sofi_var_t[reg["sofi_prop_base_t"]["type"]["value"]]["byte_num"]
        # mdb_addr reading
        mdb_addr = reg["sofi_prop_mdb_t"]["mdb_addr"]["value"]
        if isinstance(mdb_addr, str):
            if mdb_addr != "auto":
                Proj.errors["err_msg"].append("register \"{}\" in struct \"{}\" ModBUS address is unknown value "
                                              "= \"{}\". Use digit, \"auto\" or leave cell empty".format(
                    reg_name, struct_name, mdb_addr))
                Proj.errors["err_cnt"] += 1
            else:
                # Find empty range in mdb_addr_list
                space_found = False
                mdb_addr_list.sort()
                mdb_addr_head = sofi_reg.SOFI_LIMITS["mdb_addr_min"]
                if len(mdb_addr_list) == 0:
                    space_found = True
                else:
                    for ind in range(len(mdb_addr_list)):
                        if (mdb_addr_list[ind] - mdb_addr_head) >= math.ceil(reg_byte_len / 2):
                            space_found = True
                            break
                        else:
                            mdb_addr_head = mdb_addr_list[ind] + 1
                            ind += 1
                if space_found == False:
                    if sofi_reg.SOFI_LIMITS["mdb_addr_max"] - mdb_addr_head >= math.ceil(reg_byte_len / 2):
                        space_found = True
                    else:
                        Proj.errors["err_msg"].append(
                            "register \"{}\" in struct \"{}\" ModBUS address can't be assigned".format(
                                reg_name, struct_name))
                        Proj.errors["err_cnt"] += 1
                else:
                    # If empty range is found
                    reg["sofi_prop_mdb_t"]["mdb_addr"]["value"] = mdb_addr_head
                    for i in range(0, math.ceil(reg_byte_len / 2)):
                        mdb_addr_list.append(mdb_addr_head)
                        mdb_addr_head += 1
                    mdb_addr_list.sort()

    #2.3 sofi_prop_range_t
    for reg_name in Proj.prop_list["sofi_prop_range_t"]["reg_list"]:
        reg = Proj.prop_list["sofi_prop_range_t"]["reg_list"][reg_name]
        struct_name = get_struct_of_reg(reg_name, Proj)

    #2.4 sofi_prop_save_t
    #2.4.1 Manual setted save address check
    save_addr_list = []     # Save addresses
    for reg_name in Proj.prop_list["sofi_prop_save_t"]["reg_list"]:
        reg = Proj.prop_list["sofi_prop_save_t"]["reg_list"][reg_name]
        struct_name = get_struct_of_reg(reg_name, Proj)
        reg_byte_len = int(reg["sofi_prop_base_t"]["array_len"]["value"]) * \
                       sofi_reg.sofi_var_t[reg["sofi_prop_base_t"]["type"]["value"]]["byte_num"]
        # save_addr reading
        save_addr = reg["sofi_prop_save_t"]["save_addr"]["value"]
        if isinstance(save_addr, int):
            for i in range(0, reg_byte_len):
                if save_addr not in save_addr_list:
                    save_addr_list.append(int(save_addr))
                else:
                    Proj.errors["err_msg"].append(
                        "register \"{}\" in struct \"{}\" save address conflict".format(reg_name,
                                                                                          struct_name))
                    Proj.errors["err_cnt"] += 1
                    break
                save_addr += 1
    #2.4.2 Appoint save address
    save_ind = 0
    for reg_name in Proj.prop_list["sofi_prop_save_t"]["reg_list"]:
        reg = Proj.prop_list["sofi_prop_save_t"]["reg_list"][reg_name]
        struct_name = get_struct_of_reg(reg_name, Proj)
        reg_byte_len = int(reg["sofi_prop_base_t"]["array_len"]["value"]) * \
                       sofi_reg.sofi_var_t[reg["sofi_prop_base_t"]["type"]["value"]]["byte_num"]
        # save_addr reading
        save_addr = reg["sofi_prop_save_t"]["save_addr"]["value"]
        if isinstance(save_addr, str):
            if save_addr != "auto":
                Proj.errors["err_msg"].append("register \"{}\" in struct \"{}\" save address is unknown value "
                                              "= \"{}\". Use digit, \"auto\" or leave cell empty".format(
                    reg_name, struct_name, save_addr))
                Proj.errors["err_cnt"] += 1
            else:
                # Find empty range in save_addr_list
                space_found = False
                save_addr_list.sort()
                save_addr_head = sofi_reg.SOFI_LIMITS["save_addr_min"]
                if len(save_addr_list) == 0:
                    space_found = True
                else:
                    for ind in range(len(save_addr_list)):
                        if (save_addr_list[ind] - save_addr_head) >= reg_byte_len:
                            space_found = True
                            break
                        else:
                            save_addr_head = save_addr_list[ind] + 1
                            ind += 1
                if space_found == False:
                    if sofi_reg.SOFI_LIMITS["save_addr_max"] - save_addr_head >= reg_byte_len:
                        space_found = True
                    else:
                        Proj.errors["err_msg"].append(
                            "register \"{}\" in struct \"{}\" save address can't be assigned".format(
                                reg_name, struct_name))
                        Proj.errors["err_cnt"] += 1
                else:
                    # If empty range is found
                    reg["sofi_prop_save_t"]["save_addr"]["value"] = save_addr_head
                    for i in range(0, reg_byte_len):
                        save_addr_list.append(save_addr_head)
                        save_addr_head += 1
                    save_addr_list.sort()

    #2.5 sofi_prop_access_t
    for reg_name in Proj.prop_list["sofi_prop_access_t"]["reg_list"]:
        reg = Proj.prop_list["sofi_prop_access_t"]["reg_list"][reg_name]
        struct_name = get_struct_of_reg(reg_name, Proj)
        access_lvl = reg["sofi_prop_access_t"]["access_lvl"]["value"]
        if isinstance(access_lvl, int):
            # access_lvl is digit
            if access_lvl < sofi_reg.SOFI_LIMITS["access_lvl_min"]:
                access_lvl = sofi_reg.SOFI_LIMITS["access_lvl_min"]
                reg["sofi_prop_access_t"]["access_lvl"]["value"] = access_lvl
                print(
                    Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" access_lvl is too low and changed "
                                                 "to {}".format(reg_name, struct_name, access_lvl))
            elif access_lvl > sofi_reg.SOFI_LIMITS["access_lvl_max"]:
                access_lvl = sofi_reg.SOFI_LIMITS["access_lvl_max"]
                reg["sofi_prop_access_t"]["access_lvl"]["value"] = access_lvl
                print(
                    Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" access_lvl is too high and changed "
                                                 "to {}".format(reg_name, struct_name, access_lvl))
        elif isinstance(access_lvl, str):
            # access_lvl is define
            if access_lvl.upper() not in sofi_reg.sofi_access_lvl_t:
                access_lvl_new = list(sofi_reg.sofi_access_lvl_t.keys())[-1]
                reg["sofi_prop_access_t"]["access_lvl"]["value"] = access_lvl_new
                print(
                    Fore.YELLOW + Style.BRIGHT + "WARNING: register \"{}\" in struct \"{}\" access_lvl \"{}\" is "
                                                 "undefined and changed to \"{}\"".format(reg_name, struct_name,
                                                                                          access_lvl, access_lvl_new))

    print("Regs_handler done. Handled {} registers".format(reg_index))

def get_struct_of_reg(reg_name, Proj):
    found = False
    for struct_name in Proj.struct_list:
        struct = Proj.struct_list[struct_name]
        if reg_name in struct["reg_list"]:
            found = True
            return struct_name
    if found == False:
        return "Unknown"