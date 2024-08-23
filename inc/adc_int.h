

#ifndef ADC_H
#define ADC_H 1

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"
#include "pin_map.h"
#include "dcts.h"
#include "dcts_config.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include <math.h>
#include "main.h"

/**
  * @addtogroup ADC
  * @{
  */
extern ADC_HandleTypeDef hadc1;
/**
  * @}
  */

/*========== TYPEDEFS ==========*/



/*========== FUNCTION PROTOTYPES ==========*/

int adc_init (void);
void adc_deinit (void);
void adc_gpio_init (void);
void adc_gpio_deinit (void);
void adc_task(void const * argument);

#endif // ADC_H
