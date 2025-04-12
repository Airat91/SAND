import project_generator as generator
import colorama
from colorama import Fore, Back, Style, init
import openpyxl
import sofi_reg
import copy

def sofi_reg_h_processing(Proj):
    """
    Insert sofi_prop_xxx_t structs from sofi_reg.py to sofi_reg.h
    :param Proj: class Project (project_generator.Project)
    :return:
    """

    # 1 Read sofi_reg.py and find structs

    for prop_name in sofi_reg.sofi_prop_list:
        property = sofi_reg.sofi_prop_list[prop_name]
        if "header" in property:
            if property["header"]["type"] != "sofi_header_t":
                print(Fore.YELLOW + Style.BRIGHT + "WARNING: generator/sofi_reg.py: '{}' header is '{}' (expected "
                                                   "'sofi_header_t')\n".format(prop_name, property["header"]["type"]))
            else:
                Proj.sofi_properties["prop_name"].append(prop_name)
                Proj.sofi_properties[prop_name] = property
        else:
            Proj.errors["err_msg"].append("generator/sofi_reg.py: '{}' haven't header".format(prop_name))
    if len(Proj.errors["err_msg"]) > 0:
        Proj.print_new_errors()
        quit("Generator breaked")

    file_name = "sofi_reg_h"
    file = open(Proj.path[file_name], "r", encoding='UTF-8')
    text_lines = file.readlines()
    file.close()
    # find start of regs_description struct
    line_index = 0
    start_insert = 0
    end_insert = 0
    generator_insertion_find = False
    while line_index < len(text_lines) and generator_insertion_find == False:
        line = text_lines[line_index]
        generator_marker = generator.check_generator_descriptions(text_lines[line_index])
        if generator_marker != "none":
            # We found generator marker
            if (type(generator_marker) == dict and generator_marker["msg"] == "sofi_properties" and
                    generator_marker["action"] == "insert_start"):
                # We found correct generator_marker. Lets find end of struct
                start_insert = line_index
                end_insert = line_index + 1
                generator_marker = generator.check_generator_descriptions(text_lines[end_insert])
                while generator_marker == "none" and end_insert < len(text_lines):
                    generator_marker = generator.check_generator_descriptions(text_lines[end_insert])
                    end_insert += 1
                    if (type(generator_marker) == dict and generator_marker["msg"] == "sofi_properties" and
                            generator_marker["action"] == "insert_end"):
                        # We found end of regs_description struct
                        generator_insertion_find = True
        line_index += 1
    if generator_insertion_find == False:
        Proj.errors["err_msg"].append("Don't found #generator_message in " + file_name)
    else:
        # 1. Add sofi_properties to buffer for not corrupting file if errors
        buffer = []
        try:
            # 1.1 Write generator message
            buffer.append("// This part of file generated automatically, don't change it\n\n")

            # 1.2 Calc words len for align text
            max_spaces = [0,0]
            for param in sofi_reg.sofi_header_t:
                if len(sofi_reg.sofi_header_t[param]["type"]) > max_spaces[0]:
                    max_spaces[0] = len(sofi_reg.sofi_header_t[param]["type"])
                if len(param) > max_spaces[1]:
                    max_spaces[1] = len(param)
            for prop_name in Proj.sofi_properties["prop_name"]:
                property = Proj.sofi_properties[prop_name]
                for param in property:
                    if len(property[param]["type"]) > max_spaces[0]:
                        max_spaces[0] = len(property[param]["type"])
                    if len(param) > max_spaces[1]:
                        max_spaces[1] = len(param)
            for var_type in sofi_reg.sofi_var_t:
                if len(var_type) > max_spaces[0]:
                    max_spaces[0] = len(var_type)
                if len(sofi_reg.sofi_var_t[var_type]["comment"]) > max_spaces[1]:
                    max_spaces[1] = len(sofi_reg.sofi_var_t[var_type]["comment"])
            for acc_lvl in sofi_reg.sofi_access_lvl_t:
                if len(acc_lvl) > max_spaces[0]:
                    max_spaces[0] = len(acc_lvl)
                if len(str(sofi_reg.sofi_access_lvl_t[acc_lvl]["value"])) > max_spaces[1]:
                    max_spaces[1] = len(str(sofi_reg.sofi_access_lvl_t[acc_lvl]["value"]))

            # 1.3 Write sofi_prop enumeration
            buffer.append("typedef enum{\n")
            for prop_name in Proj.sofi_properties["prop_name"]:
                buffer.append("\t{},\n".format(prop_name.replace("_t","").upper()))
            buffer.append("}sofi_prop_enum_t;\n\n")

            # 1.4 Write sofi_var enumeration
            buffer.append("typedef enum{\n")
            for var_type in sofi_reg.sofi_var_t:
                space_0 = " "*(max_spaces[0] - len(var_type) + max_spaces[1] + 2)
                space_1 = " "*(max_spaces[1] - len(sofi_reg.sofi_var_t[var_type]["comment"]) + 1)
                buffer.append("\t{},{}// {}{}({} byte)\n".format(var_type.upper(), space_0, sofi_reg.sofi_var_t[var_type]["comment"],
                                                                 space_1, sofi_reg.sofi_var_t[var_type]["byte_num"]))
            buffer.append("}sofi_var_t;\n\n")

            # 1.4 Write sofi_access_lvl enumeration
            buffer.append("typedef enum{\n")
            for acc_lvl in sofi_reg.sofi_access_lvl_t:
                space_0 = " "*(max_spaces[0] - len(acc_lvl) + 1)
                space_1 = " "*(max_spaces[1] - len(str(sofi_reg.sofi_access_lvl_t[acc_lvl]["value"])) - 1)
                buffer.append("\t{}{}= {},{}// {}\n".format(acc_lvl.upper(), space_0, sofi_reg.sofi_access_lvl_t[acc_lvl]["value"],
                                                                 space_1, sofi_reg.sofi_access_lvl_t[acc_lvl]["comment"]))
            buffer.append("}sofi_access_lvl;\n\n")

            # 1.5 Write header to buffer
            buffer.append("typedef struct{\n")
            for param in sofi_reg.sofi_header_t:
                space_0 = " "*(max_spaces[0] - len(sofi_reg.sofi_header_t[param]["type"]) + 1)
                space_1 = " "*(max_spaces[1] - len(param) + 1)
                buffer.append("\t{}{}{};{}// {}\n".format(sofi_reg.sofi_header_t[param]["type"], space_0, param,
                                                          space_1, sofi_reg.sofi_header_t[param]["comment"]))
            buffer.append("}sofi_header_t;\n\n")

            # 1.6 Write sofi_prop_t to buffer
            for prop_name in Proj.sofi_properties["prop_name"]:
                property = Proj.sofi_properties[prop_name]
                buffer.append("typedef struct{\n")
                for param in property:
                    space_0 = " "*(max_spaces[0] - len(property[param]["type"]) + 1)
                    space_1 = " "*(max_spaces[1] - len(param) + 1)
                    buffer.append("\t{}{}{};{}// {}\n".format(property[param]["type"], space_0, param,
                                                              space_1, property[param]["comment"]))
                buffer.append("}"+"{};\n\n".format(prop_name))
        except:
            Proj.errors["err_msg"].append("Error during adding data to " + file_name)

        # 2. Rewrite sofi_reg.h file with new insertion from generator
        file = open(Proj.path[file_name], 'w', encoding='UTF-8')
        line_index = 0
        # 3. Write file before insertion
        while line_index <= start_insert:
            file.writelines(text_lines[line_index])
            line_index += 1
        line_index -= 1
        # 4. Write insertion
        if len(buffer) > 0:
            for line in buffer:
                file.writelines(line)
        # 5. Write file afer insertion
        line_index = end_insert - 1
        while line_index < len(text_lines):
            file.writelines(text_lines[line_index])
            line_index += 1
        # 6. Close modified file
        file.close()

    if len(Proj.errors["err_msg"]) > 0:
        Proj.print_new_errors()
        quit("Generator breaked")

def reg_map_module_xls_processing(Proj):
    """
    Read regs from reg_map_module.xls to Proj
    1. Read structs into Proj[struct_list]
    2. Read regs into Proj[struct_list][struct][reg_list]
    3. Read properties into Proj[prop_list]
    4. Add regs into Proj[prop_list][prop][reg_list]
    :param Proj: class Project (project_generator.Project)
    :return:
    """
    if len(Proj.struct_list) > 0:
        return
    file_name = Proj.path["reg_map_{}_xls".format(Proj.module)]
    file_xls = openpyxl.load_workbook(file_name)
    # Check struct_list existence in file
    if "struct_list" not in file_xls.sheetnames:
        Proj.errors["err_msg"].append("\"reg_map_{}.xls\": struct list not found".format(Proj.module))
        Proj.errors["err_cnt"] += 1
    # Read struct_list and check structs existence in file
    for worksheet in file_xls.worksheets:
        if worksheet.title == "struct_list":
            file_xls.active = worksheet
            break
    found = False
    for column in range(worksheet.min_column, worksheet.max_column + 1):
        if worksheet.cell(1, column).value == "Struct name":
            found = True
            break
    if found == False:
        Proj.errors["err_msg"].append("\"reg_map_{}.xls\": \"Struct name\" column not found".format(Proj.module))
        Proj.errors["err_cnt"] += 1
    struct_list_xls = []
    for row in range (2, worksheet.max_row + 1):
        struct_list_xls.append(worksheet.cell(row, column).value)
    for struct in struct_list_xls:
        if struct not in file_xls.sheetnames:
            print(Fore.YELLOW + Style.BRIGHT + "WARNING: \"reg_map_{}.xls\": struct \"{}\" define in struct_list but "
                                               "don't found in sheets\nStruct \"{}\" excluded from project"
                                               "".format(Proj.module, struct, struct))
            struct_list_xls.remove(struct)
    # Add properties to project
    for prop in sofi_reg.sofi_prop_list:
        Proj.prop_list[prop] = {
            "reg_list" : {}
        }
    # Add structs to project
    for struct_name in struct_list_xls:
        Proj.struct_list[struct_name] = {
            "name": struct_name,
            "reg_list": {},
        }
        project_struct = Proj.struct_list[struct_name]
        # Find sheet of current struct
        for worksheet in file_xls.worksheets:
            if worksheet.title == struct_name:
                break
        # Read properties in first row
        property_headers = {}   # column indexes for property parameters
        property_list = {}      # list of properties founded on sheet
        column = 1
        max_column = worksheet.max_column
        while column < (max_column + 1):
            property = worksheet.cell(1, column).value
            if property != None:
                property = property.lower() + "_t"
                if property in sofi_reg.sofi_prop_list:
                    property_list[property] = copy.copy(sofi_reg.sofi_prop_list[property])
                    property_list[property]["is_exist"] = False
                    # Read property parameters and save its indexes into property_headers
                    param = worksheet.cell(2, column).value
                    if param not in property_list[property]:
                        avl_list = list(property_list[property].keys())
                        avl_list.remove("header")
                        print(Fore.YELLOW + Style.BRIGHT + "WARNING: \"reg_map_{}.xls\" struct \"{}\" property \"{}\" "
                                                           "param \"{}\" is unknown.\nAvailable parameters defined in "
                                                           "\"sofi_reg.py\": {}".format(Proj.module, struct, property,
                                                                                        param, avl_list))
                    else:
                        property_headers[column] = {
                            "property": property,
                            "param": param,
                        }
                    column += 1
                    while ((worksheet.cell(1, column).value == None) and (column < (max_column + 1))):
                        param = worksheet.cell(2, column).value
                        if param not in property_list[property]:
                            avl_list = list(property_list[property].keys())
                            avl_list.remove("header")
                            print(
                                Fore.YELLOW + Style.BRIGHT + "WARNING: \"reg_map_{}.xls\" struct \"{}\" property \"{}\" "
                                                             "param \"{}\" is unknown.\nAvailable parameters defined in "
                                                             "\"sofi_reg.py\": {}".format(Proj.module, struct, property,
                                                                                          param, avl_list))
                        else:
                            property_headers[column] = {
                                "property": property,
                                "param": param,
                            }
                        column += 1
                    column -= 1
            column += 1
        # Read regs on sheet
        for row in range(3, worksheet.max_row + 1):
            reg = copy.deepcopy(property_list)
            for column in range(1, max_column):
                property = property_headers[column]["property"]
                param = property_headers[column]["param"]
                reg[property][param]["value"] = worksheet.cell(row, column).value
                if worksheet.cell(row, column).value != None:
                    reg[property]["is_exist"] = True
            reg_name = reg["sofi_prop_base_t"]["name"]["value"]
            project_struct["reg_list"][reg_name] = reg
        print("Struct \"{}\" found {} registers".format(struct_name, len(project_struct["reg_list"])))
        for reg_name in project_struct["reg_list"]:
            reg = project_struct["reg_list"][reg_name]
            for property_name in reg:
                property = reg[property_name]
                # Add regs to Proj.prop_lists
                if property["is_exist"] == True:
                    #reg["struct_name"] = struct_name
                    Proj.prop_list[property_name]["reg_list"][reg_name] = reg
    print("reg_map_module_xls_processing done")





def regs_module_h_processing(Proj):
    """
    Write project structs into regs_module.h
    :param Proj: class Project (project_generator.Project)
    :return:
    """
    return True

def regs_module_c_processing(Proj):
    """
    Write project regs declarations into regs_module.c
    :param Proj: class Project (project_generator.Project)
    :return:
    """
    return True