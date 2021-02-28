#include "delay.h"
#include "stm32f4xx.h"


void delay_ms(uint32_t tv)
{
    HAL_Delay(tv);
}
