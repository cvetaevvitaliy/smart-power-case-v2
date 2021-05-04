#ifndef SMART_POWER_CASE_V2_STM32_INIT_H
#define SMART_POWER_CASE_V2_STM32_INIT_H
#include "stm32f4xx.h"

void STM32_Init(void);

void USB_Reset(void);


/**
 * @brief  Ger RTC Handle Structure definition
 * @return pointer to RTC_HandleTypeDef struct
 */
RTC_HandleTypeDef *STM32_GetRtcHandle(void);

/**
 * @brief  Ger I2C Handle Structure definition
 * @return pointer to RTC_HandleTypeDef struct
 */
I2C_HandleTypeDef *STM32_GetI2CHandle(void);


#endif //SMART_POWER_CASE_V2_STM32_INIT_H
