/* 
 * File:        service.h
 * Authors:     Girfanov.Ayrat@yandex.ru
 * Description: Service tasks descriptions
 * Revision history: 0.1
 */

#ifndef SERVICE_H
#define	SERVICE_H

/*add includes below */
#include "type_def.h"
#include "cmsis_os.h"
/*add includes before */

#ifdef	__cplusplus
extern "C" {
#endif
    
/**
 * @defgroup service
 * @brief Service tasks descriptions
 */
    
//--------Defines--------

//--------Macro--------

//--------Typedefs-------

/**
 * @brief Enumeration for service status register flags
 */
typedef enum{
    SRV_ST_TIMEOUT_MASK = 0xFFFF,
    SRV_ST_CREATED  = (1 << 16),
    SRV_ST_RUN      = (1 << 17),
    SRV_ST_ERROR    = (1 << 18),
}service_state_t;

//-------External variables------

//-------Function prototypes----------

/**
 * @brief Check service state register status
 * @param p_service_state - pointer to service_state reg in service struct
 * @return  0 - ok,\n
 *          -1 - service isn't created,\n
 *          -2 - service in ERROR state,\n
 *          -2 - service isn't running,\n
 *
 * Check service state register with waiting timeout if service not running yet
 */
int service_is_ok(u32* p_service_state);

#ifdef __cplusplus
}
#endif

#endif /* SERVICE_H */
