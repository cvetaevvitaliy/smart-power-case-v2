#include "display.h"
#include "st7735s.h"
#include "stm32f4xx.h"
#include "tinyprintf.h"
#include "power.h"
#include "battery.h"
#include "cli.h"
#include "bq27441.h"

extern SPI_HandleTypeDef             hspi1;

static uint8_t SPI_Transmit(uint8_t *pData, uint16_t Size)
{
    if (HAL_SPI_Transmit(&hspi1, pData, Size,0) == HAL_OK)
        return true;
    else
        return false;
}


static void GPIO_Write(uint32_t port, uint32_t pin, uint8_t state)
{
    HAL_GPIO_WritePin( (GPIO_TypeDef*)port, pin, state);
}


static LCD_ST7735_ctx_t LCD_ST7735 = {
        .spi_write_data         = SPI_Transmit,     // register callback for write data to SPI
        .gpio_write_pin         = GPIO_Write,       // register callback for write GPIO pin
        .reset.gpio_port        = (uint32_t) SPI_RST_GPIO_PORT,
        .reset.gpio_pin         = SPI_RST_PIN,
        .data.gpio_port         = (uint32_t) SPI_DC_GPIO_PORT,
        .data.gpio_pin          = SPI_DC_PIN,
        .cs.gpio_port           = (uint32_t) SPI1_CS_GPIO_PORT,
        .cs.gpio_pin            = SPI1_CS_PIN,
        .backlight.gpio_port    = (uint32_t) LCD_BLK_GPIO_PORT,
        .backlight.gpio_pin     = LCD_BLK_PIN,

};


void Display_Init(void)
{
    HAL_GPIO_WritePin(LCD_EN_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_SET);
    LCD_ST7735S_Init(&LCD_ST7735);
    LCD_ST7735S_Update();

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
