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
#include "gui_settings_screen.h"
#include "gui_setup_buzzer.h"
#include "gui_setup_timer.h"
#include "gui_setup_acc.h"

static lv_indev_t * indev_keypad;
lv_group_t*  group;
static lv_disp_drv_t disp_drv;

extern gui_screen_t gui_screen;

/** Global styles */
gui_style_t gui_style;


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
    lv_obj_t *obj = lv_group_get_focused(cur_group);

    gui_settings_scr_set_actual_focus(obj);

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


    lv_indev_t *cur_drv = NULL;
    while (1) {
        cur_drv = lv_indev_get_next(cur_drv);
        if (!cur_drv) {
            break;
        }

        if (cur_drv->driver.type == LV_INDEV_TYPE_KEYPAD) {
            lv_indev_set_group(cur_drv, group);
        }

    }

    gui_style_init(&gui_style);

    gui_main_screen();

    gui_menu_screen();

    gui_settings_screen();

    gui_setup_buzzer();

    gui_setup_timer();

    gui_setup_acc();

}


lv_group_t* gui_get_group_focus_obj(void)
{
    return group;
}

_unused
char* gui_debug_event(lv_event_t event)
{
#ifdef DUBUG_EVENT
    static char* event_str = NULL;

    switch (event)
    {
        case LV_EVENT_PRESSED:
            event_str = "LV_EVENT_PRESSED";
            break;
        case LV_EVENT_PRESSING:
            event_str = "LV_EVENT_PRESSING";
            break;
        case LV_EVENT_PRESS_LOST:
            event_str = "LV_EVENT_PRESS_LOST";
            break;
        case LV_EVENT_SHORT_CLICKED:
            event_str = "LV_EVENT_SHORT_CLICKED";
            break;
        case LV_EVENT_LONG_PRESSED:
            event_str = "LV_EVENT_LONG_PRESSED";
            break;
        case LV_EVENT_LONG_PRESSED_REPEAT:
            event_str = "LV_EVENT_LONG_PRESSED_REPEAT";
            break;
        case LV_EVENT_CLICKED:
            event_str = "LV_EVENT_CLICKED";
            break;
        case LV_EVENT_RELEASED:
            event_str = "LV_EVENT_RELEASED";
            break;
        case LV_EVENT_DRAG_BEGIN:
            event_str = "LV_EVENT_DRAG_BEGIN";
            break;
        case LV_EVENT_DRAG_END:
            event_str = "LV_EVENT_DRAG_END";
            break;
        case LV_EVENT_KEY:
            event_str = "LV_EVENT_KEY";
            break;
        case LV_EVENT_FOCUSED:
            event_str = "LV_EVENT_FOCUSED";
            break;
        case LV_EVENT_DEFOCUSED:
            event_str = "LV_EVENT_DEFOCUSED";
            break;
        case LV_EVENT_LEAVE:
            event_str = "LV_EVENT_LEAVE";
            break;
        case LV_EVENT_VALUE_CHANGED:
            event_str = "LV_EVENT_VALUE_CHANGED";
            break;
        case LV_EVENT_INSERT:
            event_str = "LV_EVENT_INSERT";
            break;
        case LV_EVENT_REFRESH:
            event_str = "LV_EVENT_REFRESH";
            break;
        case LV_EVENT_APPLY:
            event_str = "LV_EVENT_APPLY";
            break;
        case LV_EVENT_CANCEL:
            event_str = "LV_EVENT_CANCEL";
            break;

        default:
            event_str = "UNKNOWN";
            break;
    }


    return event_str;
#else
    return "";
#endif

}


