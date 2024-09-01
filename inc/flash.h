#ifndef FLASH_H
#define FLASH_H
#include "sand_config.h"

#if FLASH_INT_EN
#include "flash_int.h"
#elif FLASH_EXT_EN
#include "flash_ext.h"
#endif // FLASHINT_EN/FLASH_EXT_EN

#endif // FLASH_H
