#include "gui_settings_screen.h"
#include "gui_menu_screen.h"
#include "gui_main_screen.h"
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

gui_settings_scr_t gui_obj_settings_scr;

static lv_obj_t* top_bar_str;
static lv_obj_t* top_bar;

static void event_settings_scr(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("event_settings_scr: %s\n", gui_debug_event(event));

    switch (event)
    {
        case LV_EVENT_SHORT_CLICKED:

            /** Remove all object in focus joystick and add new objects for focus */
            lv_group_remove_all_objs(gui_get_focus_obj());
            lv_group_add_obj(gui_get_focus_obj(), gui_screen.menu_screen);
            lv_group_add_obj(gui_get_focus_obj(), gui_screen.icon_1);
            lv_group_add_obj(gui_get_focus_obj(), gui_screen.icon_2);
            lv_group_add_obj(gui_get_focus_obj(), gui_screen.icon_3);
            lv_scr_load_anim(gui_screen.menu_screen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 100, false);
            break;

        default:
            break;
    }

}

void gui_settings_screen(void)
{
    /** Declare images for icons  */
    LV_IMG_DECLARE(buzzer_icon);

    /** Create menu screen */
    gui_screen.settings_screen = lv_obj_create(NULL, NULL);
    lv_obj_set_event_cb(gui_screen.settings_screen, event_settings_scr);   /*Assign an event callback*/
    lv_obj_add_style(gui_screen.settings_screen, LV_OBJ_PART_MAIN, &gui_style.style_screen);


    /** Create top bar with the style_top_bar */
    top_bar = lv_obj_create(gui_screen.settings_screen, NULL);
    lv_obj_set_size(top_bar, 160,20);
    lv_obj_add_style(top_bar, LV_OBJ_PART_MAIN, &gui_style.style_top_bar);
    lv_obj_align(top_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    /** Create top bar string */
    top_bar_str = lv_label_create(gui_screen.settings_screen, NULL);
    lv_label_set_text(top_bar_str, "Settings");
    lv_obj_set_style_local_text_color(top_bar_str, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);

    /** Create Buzzer icons */
    gui_obj_settings_scr.buzzer_icon = lv_img_create(gui_screen.settings_screen, NULL);
    lv_img_set_src(gui_obj_settings_scr.buzzer_icon, &buzzer_icon);
    lv_obj_align_origo(gui_obj_settings_scr.buzzer_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 28, -31);


    /** Applied Gum-like style for icons in menu */
    lv_obj_add_style(gui_obj_settings_scr.buzzer_icon, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);


}


void gui_settings_screen_enable_focus(void)
{
    lv_group_t* set_group = gui_get_focus_obj();

    lv_group_add_obj(set_group, gui_screen.settings_screen);
    lv_group_add_obj(set_group, gui_obj_settings_scr.buzzer_icon);

}
