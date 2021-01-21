#include "display.h"
#include "st7735s.h"
#include "stm32f4xx.h"
#include "tinyprintf.h"
#include "power.h"
#include "battery.h"
#include "cli.h"
#include "bq27441.h"

void Display_Init(void)
{
    LCD_ST7735S_Init();
    LCD_ST7735S_Update();
    HAL_GPIO_WritePin(LCD_EN_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_SET);

}

void Display_LoopService(void)
{
    Power_Status_t *power = Power_GetStatus();
    Battery_status_t *battery = Battery_GetStatus();

    static uint32_t cnt = 0;
    cnt++;
    char str[50];
    sprintf(str, "Test %d", cnt);
    LCD_ST7735_DrawString(str,0,0, &Font_8x10, ST7735_WHITE);

    sprintf(str, "Vbat %.2fV", (float)power->vbat/1000.f);
    LCD_ST7735_DrawString(str,0,10, &Font_8x10, ST7735_WHITE);

    sprintf(str, "Vbus %.2fV", (float)power->vbus/1000.f);
    LCD_ST7735_DrawString(str,0,20, &Font_8x10, ST7735_WHITE);

    sprintf(str, "Vbus_type %d", power->vbus_type);
    LCD_ST7735_DrawString(str,0,30, &Font_8x10, ST7735_WHITE);

    sprintf(str, "Current %dmA", battery->current);
    LCD_ST7735_DrawString(str,0,40, &Font_8x10, ST7735_WHITE);

    sprintf(str, "vbat %.2fV", battery->vbat);
    LCD_ST7735_DrawString(str,0,50, &Font_8x10, ST7735_WHITE);

    sprintf(str, "capacity %dmAH", battery->capacity);
    LCD_ST7735_DrawString(str,0,60, &Font_8x10, ST7735_WHITE);

    LCD_ST7735S_Update();


}
