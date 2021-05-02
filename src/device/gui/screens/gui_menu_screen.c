#include "gui_menu_screen.h"
#include "gui_main_screen.h"
#include "gui_settings_screen.h"
#include "gui.h"
#include "st7735s.h"
#include "stm32f4xx.h"
#include "tinyprintf.h"
#include "power.h"
#include "battery.h"
#include "cli.h"
#include "bq27441.h"
#include "button.h"

extern gui_screen_t gui_screen;
extern gui_style_t gui_style;
extern lv_group_t*  group;
extern gui_settings_scr_t gui_obj_settings_scr;

static lv_obj_t* top_bar_str = NULL;
static lv_obj_t* top_bar = NULL;



static void event_menu_scr(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__, gui_debug_event(event));
    static bool first_in = false;

    switch (event)
    {
        case LV_EVENT_FOCUSED:

            if(first_in == false) {
                lv_label_set_text(top_bar_str, "Menu");
                first_in = true;
            }
            else
                lv_label_set_text(top_bar_str, "Exit");

            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            first_in = false;

            /** Remove all object in focus joystick and add new objects for focus */
            lv_group_remove_all_objs(gui_get_group_focus_obj());
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.bat_percent);
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.main_scr);
            lv_scr_load_anim(gui_screen.main_scr, LV_SCR_LOAD_ANIM_MOVE_TOP, 660, 0, false);
            break;

        case LV_EVENT_LONG_PRESSED:
            break;

        default:
            break;

    }
}

static void event_handler_icon_1(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__ , gui_debug_event(event));

    switch(event)
    {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Settings");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            lv_group_remove_all_objs(gui_get_group_focus_obj());
            gui_settings_screen_enable_focus();
            lv_scr_load_anim(gui_screen.settings_scr, LV_SCR_LOAD_ANIM_OVER_RIGHT, 500, 100, false);
            break;

        default:
            break;
    }
}

static void event_handler_icon_2(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__, gui_debug_event(event));

    switch (event)
    {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Statistics");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            break;

        default:
            break;
    }
}

static void event_handler_icon_3(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__ , gui_debug_event(event));

    switch(event)
    {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Battery health");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            break;

        default:
            break;
    }
}

void gui_menu_screen(void)
{
    /** Declare images for icons  */
    LV_IMG_DECLARE(settings_icon);
    LV_IMG_DECLARE(statistics_icon);
    LV_IMG_DECLARE(battery_health_icon);

    /** Create menu screen */
    gui_screen.menu_scr = lv_obj_create(NULL, NULL);
    lv_obj_set_event_cb(gui_screen.menu_scr, event_menu_scr);   /*Assign an event callback*/
    lv_obj_add_style(gui_screen.menu_scr, LV_OBJ_PART_MAIN, &gui_style.style_screen);
    lv_scr_load_anim(gui_screen.menu_scr, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 660, 0, false);

    /** Create top bar with the style_top_bar */
    top_bar = lv_obj_create(gui_screen.menu_scr, NULL);
    lv_obj_set_size(top_bar, 160,20);
    lv_obj_add_style(top_bar, LV_OBJ_PART_MAIN, &gui_style.style_top_bar);
    lv_obj_align(top_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    /** Create top bar string */
    top_bar_str = lv_label_create(gui_screen.menu_scr, NULL);
    lv_label_set_text(top_bar_str, "Menu");
    lv_obj_set_style_local_text_color(top_bar_str, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);

    /** Create image icons in menu */
    gui_screen.icon_1 = lv_img_create(gui_screen.menu_scr, NULL);
    lv_img_set_src(gui_screen.icon_1, &settings_icon);
    lv_obj_align_origo(gui_screen.icon_1, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 28, -31);

    gui_screen.icon_2 = lv_img_create(gui_screen.menu_scr, NULL);
    lv_img_set_src(gui_screen.icon_2, &statistics_icon);
    lv_obj_align_origo(gui_screen.icon_2, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 78, -31);

    gui_screen.icon_3 = lv_img_create(gui_screen.menu_scr, NULL);
    lv_img_set_src(gui_screen.icon_3, &battery_health_icon);
    lv_obj_align_origo(gui_screen.icon_3, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 127, -31);

    /** Register event callbacks */
    lv_obj_set_event_cb(gui_screen.icon_1, event_handler_icon_1);
    lv_obj_set_event_cb(gui_screen.icon_2, event_handler_icon_2);
    lv_obj_set_event_cb(gui_screen.icon_3, event_handler_icon_3);


    /** Applied Gum-like style for icons in menu */
    lv_obj_add_style(gui_screen.icon_1, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);
    lv_obj_add_style(gui_screen.icon_2, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);
    lv_obj_add_style(gui_screen.icon_3, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);


}
