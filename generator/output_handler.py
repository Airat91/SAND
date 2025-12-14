import project_generator as generator
import colorama
from colorama import Fore, Back, Style, init
import openpyxl
import os
import copy

def mdb_space_rst_generate(Proj):
    csv_table_header = (".. csv-table::\n\t:header: \"Имя\",\"Тип\",\"Индекс\",\"Адрес ModBUS\",\"Только чтение\","
                        "\"Описание\"\n\t:widths: 45, 20, 30, 20, 30, 40\n\n")

    mdb_space_module_rst = "mdb_space_{}_rst".format(Proj.module)

    # Create output/rst directory if not exist
    if not (os.path.isdir(Proj.path["output_dir_rst"])):
        os.mkdir(Proj.path["output_dir_rst"])

    rst_file = open(Proj.path[mdb_space_module_rst], 'w+', encoding='UTF-8')
    header = "Адресное пространство {}\n".format(Proj.name)
    headline = '=' * len(header) + "\n"
    rst_file.writelines(header)
    rst_file.writelines(headline)
    header = "\nVersion-{}.{}.{}\n".format(Proj.version[0], Proj.version[1], Proj.version[2])
    rst_file.writelines(header)

    struct_list = Proj.struct_list

    for struct_name in struct_list:
        header = "\n" + struct_name.upper() + "\n"
        headline = '-' * len(header) + "\n"
        rst_file.writelines(header)
        rst_file.writelines(headline)

        reg_list = Proj.struct_list[struct_name]['reg_list']
        rst_file.writelines(csv_table_header)

        for reg_name in reg_list:
            reg = reg_list[reg_name]
            if "sand_prop_mdb_t" in reg.prop_list:
                reg_type = reg.type
                reg_ind = reg.prop_list["sand_prop_base_t"]["ind"]["value"]
                mdb_addr = reg.prop_list["sand_prop_mdb_t"]["mdb_addr"]["value"]
                reg_read_only = "RW"
                if reg.prop_list["sand_prop_base_t"]["read_only"]["value"] == 1:
                    reg_read_only = "RO"
                descr = reg.prop_list["sand_prop_base_t"]["description"]["value"]
                rst_file.writelines(
                    "\t\"{}\",\"{}\",{},{},\"{}\",{}\n".format(reg_name, reg_type, reg_ind, mdb_addr,
                                                                      reg_read_only, descr))

    rst_file.close()

    return 0