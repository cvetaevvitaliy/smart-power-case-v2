#include "st7735s_spi.h"
#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef             hspi1;

void ST7735S_SPI_Transmit(uint8_t *pData, uint16_t Size)
{
    HAL_SPI_Transmit(&hspi1, pData, Size,0);
}