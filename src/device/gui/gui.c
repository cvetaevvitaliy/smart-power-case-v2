#include "gui.h"
#include "st7735s.h"
#include "stm32f4xx.h"
#include "tinyprintf.h"
#include "power.h"
#include "battery.h"
#include "cli.h"
#include "bq27441.h"
#include "lvgl.h"
#include "button.h"
#include "gui_main_screen.h"
#include "gui_menu_screen.h"

static lv_indev_t * indev_keypad;
lv_group_t*  group;
static lv_obj_t * tv;
static lv_disp_drv_t disp_drv;


static void ST7735_flush(lv_disp_drv_t * drv, const lv_area_t * area,  lv_color_t * color_map)
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

static void focus_cb(lv_group_t * cur_group)
{
    lv_obj_t * obj = lv_group_get_focused(cur_group);
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

void gui_init(void)
{
    lv_init();

    static lv_disp_buf_t disp_buf;
    static lv_color_t buf_1[LV_HOR_RES_MAX * 10];
    lv_disp_buf_init(&disp_buf, buf_1, NULL, LV_HOR_RES_MAX * 10);

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

    gui_main_screen();

    gui_menu_screen();

}


