#include "display.h"
#include "st7735s.h"
#include "stm32f4xx.h"
#include "tinyprintf.h"
#include "power.h"
#include "battery.h"
#include "cli.h"
#include "bq27441.h"
#include "lvgl.h"
#include "button.h"
#include "gui.h"

extern SPI_HandleTypeDef             hspi1;
lv_indev_t * indev_keypad;

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


static lv_group_t*  group;
static lv_obj_t * tv;
static void msgbox_event_cb(lv_obj_t * msgbox, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED) {
        uint16_t b = lv_msgbox_get_active_btn(msgbox);
        if(b == 0 || b == 1) {
            lv_obj_del(msgbox);
            lv_obj_reset_style_list(lv_layer_top(), LV_OBJ_PART_MAIN);
            lv_obj_set_click(lv_layer_top(), false);
            lv_event_send(tv, LV_EVENT_REFRESH, NULL);
        }
    }
}


static void msgbox_create(void)
{
    static lv_style_t style;
    lv_style_init(&style);

    /*Set a background color and a radius*/
    lv_style_set_radius(&style, LV_STATE_DEFAULT, 8);
    lv_style_set_bg_opa(&style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);


    /*Add a shadow*/
    lv_style_set_shadow_width(&style, LV_STATE_DEFAULT, 15);
    lv_style_set_shadow_color(&style, LV_STATE_DEFAULT, LV_COLOR_CYAN);
    lv_style_set_shadow_ofs_x(&style, LV_STATE_DEFAULT, 2);
    lv_style_set_shadow_ofs_y(&style, LV_STATE_DEFAULT, 2);

    lv_obj_t * mbox = lv_msgbox_create(lv_layer_top(), NULL);
    lv_obj_add_style(mbox, LV_OBJ_PART_MAIN, &style);

    lv_msgbox_set_text(mbox, "Demo msg box");
    lv_obj_set_event_cb(mbox, msgbox_event_cb);
    lv_group_add_obj(group, mbox);
//    lv_group_focus_obj(mbox);
//    lv_group_set_editing(group, true);
//    lv_group_focus_freeze(group, true);

    static const char * btns[] = {"Yes", "No", ""};
    lv_msgbox_add_btns(mbox, btns);
    lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 5);

    lv_obj_set_style_local_pad_inner(mbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_pad_top(mbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_pad_bottom(mbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 3);
    //lv_obj_set_style_local_bg_color(mbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_set_style_local_bg_opa(lv_layer_top(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_70);
    lv_obj_set_style_local_bg_color(lv_layer_top(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_obj_set_click(lv_layer_top(), true);
}



static void event_handler_sw(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        ULOG_DEBUG("State: %s\n", lv_switch_get_state(obj) ? "On" : "Off");
    }
}


void Display_Init(void)
{
    HAL_GPIO_WritePin(LCD_EN_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_SET);
    LCD_ST7735S_Init(&LCD_ST7735);
    LCD_ST7735S_Update();

    gui_init();

//    /* Create a button, then set its position and event callback */
//    lv_obj_t *btn = lv_btn_create(lv_scr_act(), NULL);
//    lv_obj_set_size(btn, 30, 20);
//    //lv_obj_set_event_cb(btn, btn_event_cb);
//    lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 20);
//
//    lv_obj_t *btn2 = lv_btn_create(lv_scr_act(), NULL);
//    lv_obj_set_size(btn2, 30, 20);
//    //lv_obj_set_event_cb(btn, btn_event_cb);
//    lv_obj_align(btn2, NULL, LV_ALIGN_IN_TOP_LEFT, 40, 20);
//
//    lv_obj_t *btn3 = lv_btn_create(lv_scr_act(), NULL);
//    lv_obj_set_size(btn3, 30, 20);
//    //lv_obj_set_event_cb(btn, btn_event_cb);
//    lv_obj_align(btn3, NULL, LV_ALIGN_IN_TOP_LEFT, 80, 50);



    //msgbox_create();

    //lv_ex_roller_1();

    //lv_ex_slider_1();


//    /*Create a switch and apply the styles*/
//    lv_obj_t *sw1 = lv_switch_create(lv_scr_act(), NULL);
//    lv_obj_align(sw1, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 15);
//    lv_obj_set_event_cb(sw1, event_handler_sw);
//    lv_group_add_obj(group, sw1);


}

void Display_Loop(void)
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
