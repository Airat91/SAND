import project_generator as generator
import colorama
from colorama import Fore, Back, Style, init
import sofi_reg
import copy

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
