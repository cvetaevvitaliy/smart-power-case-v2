#include "gui_main_screen.h"
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

gui_screen_t gui_screen = {0};
extern gui_style_t gui_style;

extern lv_group_t*  group;
static char bat_perc[100] = {0};

typedef enum {
    GUI_PRINT_VOLTAGE = 1,
    GUI_PRINT_PERCENT,
    GUI_PRINT_MAH,
    GUI_PRINT_TIME,
    GUI_PRINT_CURRENT,

    GUI_LAST_VALUE,
} gui_print_bat_st_t;

gui_print_bat_st_t gui_print_bat_st;

static void my_event_cb_scr_1(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__ , gui_debug_event(event));

    switch(event)
    {
        case LV_EVENT_FOCUSED:
            ULOG_DEBUG("LV_EVENT_FOCUSED\n");
            ULOG_DEBUG("Load screen 2\n");
            lv_scr_load_anim(gui_screen.main_scr, LV_SCR_LOAD_ANIM_MOVE_TOP, 660, 0, false);
            break;
        case LV_EVENT_SHORT_CLICKED:
            ULOG_DEBUG("LV_EVENT_SHORT_CLICKED\n");
            break;
        case LV_EVENT_LONG_PRESSED:
            ULOG_DEBUG("LV_EVENT_LONG_PRESSED\n");
            break;

        default:
            break;

    }
}

static void event_main_scr(lv_obj_t * obj, lv_event_t event)
{
    ULOG_DEBUG("%s: %s\n", __FUNCTION__ , gui_debug_event(event));

    switch(event)
    {
        case LV_EVENT_FOCUSED:
            lv_event_send(gui_screen.bat_percent, LV_EVENT_FOCUSED, NULL);

            //lv_scr_load_anim(gui_screen.main_scr_1, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 660, 0, false);
            //lv_scr_load_anim(gui_screen.main_scr_2, LV_SCR_LOAD_ANIM_MOVE_TOP, 660, 0, false);
            break;
        case LV_EVENT_SHORT_CLICKED:

            /** Remove all object in focus joystick and add new objects for focus */
            lv_group_remove_all_objs(gui_get_group_focus_obj());
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.menu_scr);
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.icon_1);
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.icon_2);
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.icon_3);
            lv_scr_load_anim(gui_screen.menu_scr, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 660, 0, false);
            break;
        case LV_EVENT_LONG_PRESSED:
            Power_PowerOff(POWER_OFF_ALL);
            break;

        default:
            break;

    }
}

static void event_handler_bat_info(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__ , gui_debug_event(event));

    switch(event)
    {
        case LV_EVENT_FOCUSED:
            gui_print_bat_st++;
            if (gui_print_bat_st > GUI_LAST_VALUE)
                gui_print_bat_st = GUI_PRINT_VOLTAGE;

            break;

        case LV_EVENT_SHORT_CLICKED:
            lv_event_send(gui_screen.main_scr, LV_EVENT_SHORT_CLICKED, NULL);
            break;
        case LV_EVENT_LONG_PRESSED:
            lv_event_send(gui_screen.main_scr, LV_EVENT_LONG_PRESSED, NULL);
            break;
    }
}

void gui_main_screen(void)
{
    LV_IMG_DECLARE(battery_1);
    LV_IMG_DECLARE(battery_icon_big);
    LV_IMG_DECLARE(battery_icon_small);
    LV_IMG_DECLARE(mah_icon);
    LV_IMG_DECLARE(big_bat_tail_icon);

    gui_screen.main_scr = lv_obj_create(NULL, NULL);
    lv_obj_set_event_cb(gui_screen.main_scr, event_main_scr);   /*Assign an event callback*/
    lv_obj_add_style(gui_screen.main_scr, LV_OBJ_PART_MAIN, &gui_style.style_screen);
    lv_scr_load_anim(gui_screen.main_scr, LV_SCR_LOAD_ANIM_NONE, 660, 0, false);


    gui_screen.icon_bat_big = lv_obj_create(gui_screen.main_scr, NULL);
    lv_obj_set_size(gui_screen.icon_bat_big, 108,56);
    lv_obj_add_style(gui_screen.icon_bat_big, LV_OBJ_PART_MAIN, &gui_style.style_big_bat);
    lv_obj_align(gui_screen.icon_bat_big, NULL, LV_ALIGN_CENTER, -6, 0);

    lv_obj_t * icon_bat_tail;
    icon_bat_tail = lv_img_create(gui_screen.main_scr, NULL);
    lv_img_set_src(icon_bat_tail, &big_bat_tail_icon);
    lv_obj_align_origo(icon_bat_tail, NULL, LV_ALIGN_IN_RIGHT_MID, -23, 0);

    gui_screen.fill_icon_bat_big = lv_obj_create(gui_screen.main_scr, NULL);
    lv_obj_set_size(gui_screen.fill_icon_bat_big, 100,48);
    lv_obj_add_style(gui_screen.fill_icon_bat_big, LV_OBJ_PART_MAIN, &gui_style.style_fill_bat);
    lv_obj_align(gui_screen.fill_icon_bat_big, NULL, LV_ALIGN_CENTER, -6, 0);


    gui_screen.bat_percent = lv_label_create(gui_screen.main_scr, NULL);
    lv_label_set_text(gui_screen.bat_percent, bat_perc);
    lv_obj_set_style_local_text_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_sel_bg_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_set_style_local_text_font(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_26);
    lv_obj_align_origo(gui_screen.bat_percent, gui_screen.fill_icon_bat_big, LV_ALIGN_CENTER, -49, -1);

    lv_label_set_long_mode(gui_screen.bat_percent, LV_LABEL_LONG_SROLL);     /*Circular scroll*/
    lv_obj_set_width(gui_screen.bat_percent, 98);
    lv_obj_set_style_local_text_letter_space(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_event_cb(gui_screen.bat_percent, event_handler_bat_info);


    lv_group_add_obj(group, gui_screen.bat_percent);
    lv_group_add_obj(group, gui_screen.main_scr);
    //lv_group_add_obj(group, gui_screen.main_scr_1);

}

void gui_set_bat_percent(uint8_t value)
{
    lv_obj_set_size(gui_screen.fill_icon_bat_big, value,48);
    if (value > 65 && value <= 100)
        lv_obj_set_style_local_bg_color(gui_screen.fill_icon_bat_big, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x20f962));
    else if (value > 20 && value <= 65)
        lv_obj_set_style_local_bg_color(gui_screen.fill_icon_bat_big, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xf5bb3d));
    else if (value > 0 && value <= 20)
        lv_obj_set_style_local_bg_color(gui_screen.fill_icon_bat_big, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);

//    sprintf(bat_perc, "%d%%,value);
//    lv_label_set_text(gui_screen.bat_percent, bat_perc);
//    if (value > 75 && value <= 100)
//        lv_obj_set_style_local_text_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
//    else if (value > 40 && value <= 75)
//        lv_obj_set_style_local_text_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
//    else
//        lv_obj_set_style_local_text_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

void gui_set_bat_voltage(float value)
{
//    sprintf(bat_perc, "%.2fV ", value);
//    lv_label_set_text(gui_screen.bat_percent, bat_perc);
//    lv_obj_align_origo(gui_screen.bat_percent, NULL, LV_ALIGN_CENTER, -1, 0);
}

void gui_update_value(void)
{
    Battery_status_t *battery_status = Battery_GetStatus();
    switch (gui_print_bat_st)
    {
        case GUI_PRINT_VOLTAGE:
            sprintf(bat_perc, "  %.2fV", battery_status->vbat);
            break;

        case GUI_PRINT_PERCENT:
        {
            if (battery_status->percent_unfiltered == 100)
                sprintf(bat_perc, "  %d%%", battery_status->percent_unfiltered);
            else if (battery_status->percent < 100 && battery_status->percent_unfiltered > 10)
                sprintf(bat_perc, "   %d%%", battery_status->percent_unfiltered);
            else
                sprintf(bat_perc, "    %d%%", battery_status->percent_unfiltered);
        }
            break;

        case GUI_PRINT_MAH:
            sprintf(bat_perc, "%dmAh", battery_status->capacity);
            break;

        case GUI_PRINT_CURRENT:
            sprintf(bat_perc, "%.3fA", (float) battery_status->current / 1000.f);
            break;

        case GUI_PRINT_TIME:
        {
            uint16_t time;
            if (battery_status->current < 0){
                time = (float) ((((float)battery_status->capacity / (float) battery_status->current) * -1) * 60);
                sprintf(bat_perc, "%dh%dmin", time / 60, time % 60);
            }
            else {
                sprintf(bat_perc, "Charging");
            }
        }
            break;

        default:
            gui_print_bat_st = GUI_PRINT_VOLTAGE;
            break;
    }

    lv_label_set_text(gui_screen.bat_percent, bat_perc);
    gui_set_bat_percent(battery_status->percent_unfiltered);

}

