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
#define BUILD_INFO "main-22-7803ee33"
#define BUILD_INFO_MAX_LEN  20      // Max length of BUILD_INFO string
// Array of software version numbers
#define BUILD_VERSION {0, 0, 0}
// Build time string like "<day>-<month>-<year>_<hour>:<min>"
#define BUILD_DATE "04-05-2025_22:11"
#define BUILD_DATE_MAX_LEN  20      // Max length of BUILD_DATE string
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* BULD_INFO_H */

