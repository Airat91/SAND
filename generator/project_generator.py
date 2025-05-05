# coding: utf_8
import datetime
#import sys
import os
import re
#import subprocess
#import fileinput
#import msvcrt as m
#import markdown
import argparse
from git import Repo
import hashlib
import json
#import math
import colorama
from colorama import Fore, Back, Style, init
#from pkg_resources import file_ns_handler
import file_handler
import regs_handler

GENERATOR = {
    "marker": "#generator_message",     #marker in file for parse by generator
    "msg": [                            #list of available messages
        "sofi_properties",                  #properties typedefs
        "sofi_struct",                      #global struct
        "sofi_struct_external",             #external struct definitions
        "sofi_struct_define",               #global struct defines
    ],
    "action": ["insert_start","insert_end","search_start","search_end"],
    "type_module": ["ai","ao","di","do","do_16","multi","bric","all","do_r_16","di_n_16","do_24", "comb_22"],
    "name": "",
}
DEVICE = {
    "sand_ai"   : {
        "device_type"   : 10,
        "device_name"   : "SAND_AI",
    }
}

compilation_max_len = 20    #max length of compilation info string
__description__ = 'Generate project'

class Project():
    def __init__(self, project_path, module_name):
        """
        Init PROJECT-object
        :param project_path: path of project GIT-repository
        :param module_name: project module name from DEVICE dict
        """
        self.name = module_name.upper(),
        self.module = module_name,
        self.path = {                   #path of files
            "project_path"              : project_path,
            "sofi_reg_h"                : project_path + "\\inc\\sofi_reg.h",
            "sofi_reg_py"               : project_path + "\\generator\sofi_reg.py",
            "build_info_h"              : project_path + "\\inc\\build_info.h",
            "output_hex"                : project_path + "\\build\\SAND.hex",
            "output_hex_dir"            : project_path + "\\build\\",
            "hash_json"                 : project_path + "\\generator\hash.json",
            "regs_module_h"             : project_path + "\\inc\\regs_{}.h".format(module_name),
            "regs_module_c"             : project_path + "\\src\\regs_{}.c".format(module_name),
            "reg_map_{}_xls".format(module_name)    : project_path + "\\generator\\input\\reg_maps\\reg_map_{}.xlsx".format(module_name),
        }
        self.version = []               #version
        self.version_str = ""           #version like string
        self.branch = ""                #head branch name
        self.errors = {                 #errors
            "err_msg": [],
            "err_cnt": 0,
        }
        self.generated_list = [         #list of generate files
            "sofi_reg_h",
            "sofi_reg_py",
            "reg_map_{}_xls".format(module_name),
            "regs_module_h",
            "regs_module_c",
        ]
        self.was_changed_list = []      #list of files for generate
        self.sofi_properties = {        #list of sofi_prop_xxx_t structs
            "prop_name":    [],         #list of property names
        }
        self.struct_list = {}           #List of structs from reg_map_module_xls
        self.prop_list = {}             #List of regs by properties

        #read version
        repo = Repo(self.path["project_path"])

        #read compilation info
        head_commit = repo.head.commit.hexsha
        head_branch = repo.head.ref.name
        self.branch = head_branch
        commits = list(repo.iter_commits('HEAD'))
        count_commits = len(commits)
        self.compiled = "{}-{}-{}".format(head_branch, count_commits, head_commit[0:8])
        if(len(self.compiled))>compilation_max_len:
            compiled_original = self.compiled
            compiled_short = self.compiled[0:compilation_max_len-1]
            self.compiled = compiled_short
            print(Fore.YELLOW + Style.BRIGHT + "WARNING: Compilation info length is too long and sliced from \"{} to {}".format(
                    compiled_original, self.compiled))

        #read version tag
        tags = []
        for tag in repo.tags:
            # individual tags for each device (not resolved yet)
            tags.append(tag)
        if len(tags) > 0:
            tags = sorted(tags, key=lambda t: t.commit.committed_datetime)
            temp = str(tags[-1]).split(".")
            for num in temp:
                self.version.append(int(num))
            print("Project version: {}.{}.{}".format(self.version[0], self.version[1], self.version[2]))

        else:
            # tags not found so set version to 0.0.0
            self.version = [0, 0, 0]
            print(Fore.YELLOW + Style.BRIGHT + "WARNING: Don't found tags in project's git repository.\n\tPlease check it"
                                             " by 'git pull origin --tags' command.\n\tAt now version resets to: {}.{}.{}".format(                    self.version[0], self.version[1], self.version[2]))
        self.version_str = "{"+"{}, {}, {}".format(self.version[0], self.version[1], self.version[2])+"}"

    def print_new_errors(self):
        for i in range(self.errors["err_cnt"], len(self.errors["err_msg"])):
            print(Fore.RED + Style.BRIGHT + "GENERATOR ERROR: {}".format(self.errors["err_msg"][i]))
            self.errors["err_cnt"] += 1

    def print_all_errors(self):
        for i in range(len(self.errors["err_msg"])):
            print(Fore.RED + Style.BRIGHT + "GENERATOR ERROR: {}".format(self.errors["err_msg"][i]))

def main():
    """
    Main generator function
    1. Parse input parameters and options
    2. Read git-repository information
    3. Check errors
    4. Preprocess: Generate project structs and handle project files
        4.1 Replace build info
        4.2 Checking project files hash and add into changed_list if changed
        4.3 If sofi_reg.py in changed_list, add sofi_reg.h into changed_list
        4.4 If sofi_reg.h in changed_list, run sofi_reg_h_processing()
        4.5 If reg_map_module.xls in changed_list, run reg_map_module_xls_processing() and add regs_module_h into changed_list
        4.6 If regs_module_h in changed_list, run regs_module_h_processing(), and add regs_module_c to changed_list
        4.7 If regs_module_c in changed_list, run regs_module_c_processing()
    5. Postprocess: Rename output HEX-file
    :return:
    """
    #1. Parse input parameters
    parser = argparse.ArgumentParser(description=__description__)
    parser.add_argument('--pre',
                        action='store_true',
                        dest='preprocess',
                        default=False,
                        help='add build info into project before build')
    parser.add_argument('--post',
                        action='store_true',
                        dest='postprocess',
                        default=False,
                        help='rename output HEX file after build')
    parser.add_argument('--force',
                        action='store_true',
                        dest='generate_force',
                        default=False,
                        help='rewrite all generated files')
    parser.add_argument('-m', '--module',
                        required=True,
                        dest='module',
                        default=False,
                        help='type of module')
    parser.add_argument('-d', '--device_type',
                        required=True,
                        dest='device_type',
                        default=False,
                        help='device_type code')
    args = parser.parse_args()

    if args.device_type != False:
        module_found = False
        for module in DEVICE:
            if args.device_type == str(DEVICE[module]["device_type"]):
                args.module = module
                module_found = True
        if module_found == False:
            device_type_list = []
            for module in DEVICE:
                device_type_list.append(DEVICE[module]["device_type"])
            quit(Fore.RED + Style.BRIGHT + "Device_type \"{}\" is unknown.\nPlease select device_type from list: {}".format(
                args.device_type, device_type_list))
    if args.module == False:
        quit(Fore.RED + Style.BRIGHT + "Expected -m MODULE_NAME or -d DEVICE_TYPE")
    elif args.module not in DEVICE:
        quit(Fore.RED + Style.BRIGHT + "Module \"{}\" is unknown.\nPlease select module from list: {}".format(args.module, list(DEVICE)))

    #2. Read repo information
    PROJECT = Project(os.path.abspath(__file__).replace("generator\project_generator.py",""), args.module)
    #print("Project version; {}.{}.{}".format(PROJECT.version[0], PROJECT.version[1], PROJECT.version[2]))
    PROJECT.name = args.module.upper()
    PROJECT.module = args.module
    print("Project \"{}\"".format(PROJECT.name))
    print("Compilation info string: {}".format(PROJECT.compiled))

    #3. Check errors
    if len(PROJECT.errors["err_msg"]) > 0:
        PROJECT.print_new_errors()
        quit("Generator breaked")

    if args.preprocess:
    #4. Pre-build stages
        #4.1 Replace build info
        replace_define(PROJECT.path["build_info_h"], "BUILD_VERSION", PROJECT.version_str)
        replace_define(PROJECT.path["build_info_h"], "BUILD_INFO", "\""+PROJECT.compiled+"\"")
        dt_now = datetime.datetime.now()
        datetime_str = "\"{:02d}-{:02d}-{:04d}_{:02d}:{:02d}\"".format(dt_now.day, dt_now.month, dt_now.year,
                                                                       dt_now.hour, dt_now.minute)
        replace_define(PROJECT.path["build_info_h"], "BUILD_DATE", datetime_str)

        #4.2 Checking project files hash and add into generated_list if changed
        if args.generate_force:
            #4.2.1 Copy all generate files to generate list
            for file_name in PROJECT.generated_list:
                PROJECT.was_changed_list.append(file_name)
        else:
            #4.2.2 Read hash.json and compare with files
            try:
                hash_json = open(PROJECT.path["hash_json"], "r", encoding='UTF-8')
                hash_data = json.load(hash_json)
            except:
                print("Can't found hash.json. Create new file...")
                hash_json = open(PROJECT.path["hash_json"], "w", encoding='UTF-8')
                hash_data = {}
                for file_name in PROJECT.generated_list:
                    hash_data[file_name] = 0
                json.dump(hash_data, hash_json, indent=4)
            hash_json.close()
            for file_name in PROJECT.generated_list:
                if hash_calc(PROJECT.path[file_name]) != hash_data[file_name]:
                    PROJECT.was_changed_list.append(file_name)
            if len(PROJECT.was_changed_list) > 0:
                print("Files {} was changed".format(PROJECT.was_changed_list))

        #4.3 If sofi_reg.py in changed list
        if "sofi_reg_py" in PROJECT.was_changed_list:
            # Rewrite hash for sofi_reg.py
            hash_json = open(PROJECT.path["hash_json"], "r", encoding='UTF-8')
            hash_data = json.load(hash_json)
            hash_data["sofi_reg_py"] = hash_calc(PROJECT.path["sofi_reg_py"])
            hash_json = open(PROJECT.path["hash_json"], "w", encoding='UTF-8')
            json.dump(hash_data, hash_json, indent=4)
            hash_json.close()
            
            # Add sofi_reg.h to changed list
            if "sofi_reg_h" not in PROJECT.was_changed_list:
                PROJECT.was_changed_list.append("sofi_reg_h")
            
        # 4.4 If sofi_reg.h in changed list
        if "sofi_reg_h" in PROJECT.was_changed_list:
            file_handler.sofi_reg_h_processing(PROJECT)
            if PROJECT.errors["err_cnt"] > 0:
                PROJECT.print_all_errors()
                quit("Generator breaked")
            # Rewrite hash for sofi_reg_h
            hash_json = open(PROJECT.path["hash_json"], "r", encoding='UTF-8')
            hash_data = json.load(hash_json)
            hash_data["sofi_reg_h"] = hash_calc(PROJECT.path["sofi_reg_h"])
            hash_json = open(PROJECT.path["hash_json"], "w", encoding='UTF-8')
            json.dump(hash_data, hash_json, indent=4)
            hash_json.close()

        #4.5 If reg_map_module_xls in changed list
        reg_map_module_xls = "reg_map_{}_xls".format(PROJECT.module)
        if reg_map_module_xls in PROJECT.was_changed_list:
            # Check reg_map_module_xls correction and read structs to PROJECT
            file_handler.reg_map_module_xls_processing(PROJECT)
            if PROJECT.errors["err_cnt"] > 0:
                PROJECT.print_all_errors()
                quit("Generator breaked")
            # Rewrite hash for reg_map_module_xls
            hash_json = open(PROJECT.path["hash_json"], "r", encoding='UTF-8')
            hash_data = json.load(hash_json)
            hash_data[reg_map_module_xls] = hash_calc(PROJECT.path[reg_map_module_xls])
            hash_json = open(PROJECT.path["hash_json"], "w", encoding='UTF-8')
            json.dump(hash_data, hash_json, indent=4)
            hash_json.close()
            regs_handler.regs_handler(PROJECT)
            if PROJECT.errors["err_cnt"] > 0:
                PROJECT.print_all_errors()
                quit("Generator breaked")

            # Add regs_module_h to changed list
            if "regs_module_h" not in PROJECT.was_changed_list:
                PROJECT.was_changed_list.append("regs_module_h")

        #4.6 If regs_module_h in changed list
        if "regs_module_h" in PROJECT.was_changed_list:
            if len(PROJECT.struct_list) == 0:
                # Read structs from reg_map_module_xls to PROJECT
                file_handler.reg_map_module_xls_processing(PROJECT)
                if PROJECT.errors["err_cnt"] > 0:
                    PROJECT.print_all_errors()
                    quit("Generator breaked")
                regs_handler.regs_handler(PROJECT)
                if PROJECT.errors["err_cnt"] > 0:
                    PROJECT.print_all_errors()
                    quit("Generator breaked")
            # Fill regs_module_h by structs from reg_map_module_xls
            file_handler.regs_module_h_processing(PROJECT)
            if PROJECT.errors["err_cnt"] > 0:
                PROJECT.print_all_errors()
                quit("Generator breaked")
            # Rewrite hash for regs_module_h
            hash_json = open(PROJECT.path["hash_json"], "r", encoding='UTF-8')
            hash_data = json.load(hash_json)
            hash_data["regs_module_h"] = hash_calc(PROJECT.path["regs_module_h"])
            hash_json = open(PROJECT.path["hash_json"], "w", encoding='UTF-8')
            json.dump(hash_data, hash_json, indent=4)
            hash_json.close()

            # Add regs_module_c to changed list
            if "regs_module_c" not in PROJECT.was_changed_list:
                PROJECT.was_changed_list.append("regs_module_c")

        #4.7 If regs_module_c in changed list
        if "regs_module_c" in PROJECT.was_changed_list:
            # Fill regs_module_c by declarations
            file_handler.regs_module_c_processing(PROJECT)
            if PROJECT.errors["err_cnt"] > 0:
                PROJECT.print_all_errors()
                quit("Generator breaked")
            # Rewrite hash for regs_module_c
            hash_json = open(PROJECT.path["hash_json"], "r", encoding='UTF-8')
            hash_data = json.load(hash_json)
            hash_data["regs_module_c"] = hash_calc(PROJECT.path["regs_module_c"])
            hash_json = open(PROJECT.path["hash_json"], "w", encoding='UTF-8')
            json.dump(hash_data, hash_json, indent=4)
            hash_json.close()

    elif args.postprocess:
    #5. Rename output HEX-file
        origin_file = open(PROJECT.path["output_hex"], "rb")
        file_name = "{}_{}_{}_{}_({}).hex".format(PROJECT. PROJECT.version[0],
            PROJECT.version[1], PROJECT.version[2], PROJECT.branch)
        output_file = PROJECT.path["output_hex_dir"] + file_name
        new_file = open(output_file, "wb")
        new_file.write(origin_file.read())
        origin_file.close()
        new_file.close()

        print("Output file: {}".format(file_name))


def replace_define(file_path, define_str, define_value):
    """
    Find "#define DEFINE_STR VALUE" string and replace value
    :param file_path: file for replace
    :param define_str: definition for looking
    :param define_value: new value for replace
    :return:
    """
    line_index = 0
    file = open(file_path, "r", encoding='UTF-8')
    text_lines = file.readlines()
    file.close()
    while line_index < len(text_lines):
        define_match = re.compile(r"^\s*#define\s+(?P<def>[\w0-9]+)\s+(?P<val>[\W\w]+)")
        if define_match.match(text_lines[line_index]):
            define = define_match.match(text_lines[line_index]).groupdict()["def"]
            value = define_match.match(text_lines[line_index]).groupdict()["val"]
            if define == define_str:
                text_lines[line_index] = "#define {} {}\n".format(define_str, define_value)
        line_index += 1
    file = open(file_path, "w", encoding='UTF-8')
    line_index = 0
    while line_index < len(text_lines):
        file.writelines(text_lines[line_index])
        line_index += 1
    file.close()

def hash_calc(file_path):
    """
    Calculate hash for file
    :param file_path: file for read
    :return: hash value like string
    """
    # check fsdata was changed?
    file = open(file_path, "rb")
    hash = hashlib.sha256()
    hash.update(file.read())
    file.close()
    return  str(hash.hexdigest())

def check_generator_descriptions(line):
    """
    Find generator_marker in line.
    :param line: string for looking
    :return:    string "none" - generator_marker not found,
                string error - generator_marker found but it incorrect,
                dict {json object} - generator_marker found and correct
    """
    if GENERATOR["marker"] in line and "{" in line and "}" in line:
        last_str = [pos for pos, char in enumerate(line) if char == "}"]
        json_part = line[line.index("{"):last_str[-1]+1]
        #Deserialise json from string
        json_obj = json.loads(json_part)

        #json parameters validation
        err_msg = json_part+"\n"
        err_found = False
        if "msg" in json_obj:
            if json_obj["msg"] not in GENERATOR["msg"]:
                err_found = True
                err_msg += "Unknown \"msg\":\"{}\" parameter in generator_marker. Available values: ".format(json_obj["msg"])
                for i in range(len(GENERATOR["msg"])):
                    err_msg += "\"" + GENERATOR["msg"][i] + "\", "
                err_msg = err_msg[:-2] + "\n"
            else:
                if json_obj["msg"] == "sofi_properties":
                    #action check
                    if "action" not in json_obj:
                        err_found = True
                        err_msg += "\"action\" parameter missing\n"
                elif json_obj["msg"] == "start_struct":
                    #name check
                    if "name" not in json_obj:
                        err_found = True
                        err_msg += "\"name\" parameter missing\n"
                    #type_module check
                    if "type_module" in json_obj:
                        if json_obj["type_module"] not in GENERATOR["type_module"]:
                            err_found = True
                            err_msg += "Unknown \"type_module\":\"{}\" parameter in generator_marker. Available values: ".format(json_obj["type_module"])
                            for i in range(len(GENERATOR["type_module"])):
                                err_msg += "\"" + GENERATOR["type_module"][i] + "\", "
                            err_msg = err_msg[:-2] + "\n"
                    else:
                        err_found = True
                        err_msg += "\"type_module\" parameter missing. Available values: "
                        for i in range(len(GENERATOR["type_module"])):
                            err_msg += "\"" + GENERATOR["type_module"][i] + "\", "
                        err_msg = err_msg[:-2] + "\n"
        else:
            err_found = True
            err_msg += "Don't found 'msg' in generator_marker\n"
        if err_found:
            return err_msg
        else:
            return json_obj
    else:
        return "none"

def get_msg_line_nmbr(Proj, file_name, generator_msg, generator_action):
    """
    Read file and return line number with generator message
    :param Proj: Project object
    :param file_name: file name in Proj.path list
    :param generator_msg: see GENERATOR["msg"] value
    :param generator_action: see  GENERATOR["action"] value
    :return:    line_number int value,
                "none" str if generator_message not found or invalid
    """
    # Check generator_message
    if generator_msg not in GENERATOR["msg"]:
        Proj.errors["err_msg"].append("\"get_msg_line_nmbr()\": generator_message \"{}\" is unknown. Available values: "
                                      "{}".format(generator_msg, GENERATOR["msg"]))
        Proj.errors["err_cnt"] += 1
        return "not_found"
    if generator_action not in GENERATOR["action"]:
        Proj.errors["err_msg"].append("\"get_msg_line_nmbr()\": generator_action \"{}\" is unknown. Available values: "
                                      "{}".format(generator_action, GENERATOR["action"]))
        Proj.errors["err_cnt"] += 1
        return "not_found"
    # Find generator_message in file
    file = open(Proj.path[file_name], "r", encoding='UTF-8')
    text_lines = file.readlines()
    file.close()
    # find start of regs_description struct
    line_index = 0
    start_insert = 0
    end_insert = 0
    generator_marker_find = False
    while line_index < len(text_lines) and generator_marker_find == False:
        line = text_lines[line_index]
        generator_marker = check_generator_descriptions(text_lines[line_index])
        if generator_marker != "none":
            # We found generator marker
            if (type(generator_marker) == dict and generator_marker["msg"] == generator_msg and
                    generator_marker["action"] == generator_action):
                generator_marker_find = True
                # We found correct generator_marker. Return line number
                return line_index
        line_index += 1
    if generator_marker_find  == False:
        return "not_found"


if __name__ == "__main__":
    main()