/* 
 * File:        build_info.h
 * Author:      Girfanov.Ayrat@yandex.ru
 * Comments:    This file save definitions of compilation info each build 
 *              operation and rewrite by "project-generator.py" script
 *              before build (run script with --pre option).
 * Revision history: 1.1
 */

#ifndef BULD_INFO_H
#define BULD_INFO_H
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
// String like "<brach name>_<commit counter>_<commit hash>"
#define BUILD_INFO "ftr/flash-84-b02af1"
#define BUILD_INFO_MAX_LEN  20      // Max length of BUILD_INFO string
// Array of software version numbers
#define BUILD_VERSION {0, 0, 4}
// Build time string like "<day>-<month>-<year>_<hour>:<min>"
#define BUILD_DATE "05-11-2025_06:36"
#define BUILD_DATE_MAX_LEN  20      // Max length of BUILD_DATE string
    
#if STM32F103xB
    #define MCU_NAME    "STM32F103CBT6"     // MCU-chip name
#endif // MCU_NAME

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* BULD_INFO_H */

