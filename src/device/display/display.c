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

void ST7735_flush(lv_disp_drv_t * drv, const lv_area_t * area,  lv_color_t * color_map)
{
    /** put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            /** put_px(x, y, *color_p)*/
            LCD_ST7735S_DrawPixel(x, y, color_map->full);
            color_map++;
        }
    }

    LCD_ST7735S_Update();
    /** IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing */
    lv_disp_flush_ready(drv);
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

void lv_tutorial_hello_world(void)
{
    /*Create a Label on the currently active screen*/
    lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);

    /*Modify the Label's text*/
    //lv_label_set_text(label1, "Hello world!");
    lv_label_set_text(label1, LV_SYMBOL_BATTERY_3);

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}

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

static void focus_cb(lv_group_t * group)
{
    lv_obj_t * obj = lv_group_get_focused(group);
    if(obj != tv) {
//        uint16_t tab = lv_tabview_get_tab_act(tv);
//        switch(tab) {
//            case 0:
//                lv_page_focus(t1, obj, LV_ANIM_ON);
//                break;
//            case 1:
//                lv_page_focus(t2, obj, LV_ANIM_ON);
//                break;
//            case 2:
//                lv_page_focus(t3, obj, LV_ANIM_ON);
//                break;
//        }
    }
}


static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        ULOG_DEBUG("Selected month: %s\n", buf);
    }
}


void lv_ex_roller_1(void)
{
    lv_obj_t *roller1 = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(roller1,
                          "January\n"
                          "February\n"
                          "March\n"
                          "April\n"
                          "May\n"
                          "June\n"
                          "July\n"
                          "August\n"
                          "September\n"
                          "October\n"
                          "November\n"
                          "December",
                          LV_ROLLER_MODE_INFINITE);

    lv_roller_set_visible_row_count(roller1, 3);
    lv_obj_align(roller1, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(roller1, event_handler);
    lv_group_add_obj(group, roller1);
}

static lv_obj_t * slider_label;
static void event_handler2(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        ULOG_DEBUG("Value: %d\n", lv_slider_get_value(obj));
        static char buf[4];
        snprintf(buf, 4, "%u", lv_slider_get_value(obj));
        lv_label_set_text(slider_label, buf);
    }
}

void lv_ex_slider_1(void)
{
    /*Create a slider*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(slider, event_handler2);

    /* Create a label below the slider */
    slider_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label, "0");
    lv_obj_set_auto_realign(slider_label, true);
    lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

    lv_group_add_obj(group, slider);

    lv_obj_t * slider2 = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_align(slider2, NULL, LV_ALIGN_CENTER, 00, 10);
    lv_obj_set_event_cb(slider2, event_handler2);
    lv_group_add_obj(group, slider2);
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
    lv_init();

    static lv_disp_buf_t disp_buf;
    static lv_color_t buf_1[LV_HOR_RES_MAX * 10];
    lv_disp_buf_init(&disp_buf, buf_1, NULL, LV_HOR_RES_MAX * 10);


    lv_disp_drv_t disp_drv;
    disp_drv.hor_res = 160;
    disp_drv.ver_res = 80;

    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = ST7735_flush;
    lv_disp_drv_register(&disp_drv);

    /** Register a keypad input device*/
    lv_indev_drv_t indev_drv;

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    lv_tutorial_hello_world();
    //lv_demo_keypad_encoder();

    group = lv_group_create();
    lv_group_set_focus_cb(group, focus_cb);

    lv_indev_t* cur_drv = NULL;
    while (1) {
        cur_drv = lv_indev_get_next(cur_drv);
        if (!cur_drv) {
            break;
        }

        if (cur_drv->driver.type == LV_INDEV_TYPE_KEYPAD) {
            lv_indev_set_group(cur_drv, group);
        }
    }

    static lv_style_t style_modal;
    lv_style_init(&style_modal);
    lv_style_set_bg_color(&style_modal, LV_STATE_DEFAULT, LV_COLOR_BLACK);

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



    msgbox_create();

    //lv_ex_roller_1();

    lv_ex_slider_1();


    /*Create a switch and apply the styles*/
    lv_obj_t *sw1 = lv_switch_create(lv_scr_act(), NULL);
    lv_obj_align(sw1, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 15);
    lv_obj_set_event_cb(sw1, event_handler_sw);
    lv_group_add_obj(group, sw1);


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
