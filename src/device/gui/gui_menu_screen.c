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
extern lv_group_t*  group;


static void event_menu_scr(lv_obj_t * obj, lv_event_t event)
{
    ULOG_DEBUG("event_menu_scr %d\n", event);

    switch(event)
    {
        case LV_EVENT_FOCUSED:
            ULOG_DEBUG("menu_scr: LV_EVENT_FOCUSED\n");
            break;
        case LV_EVENT_SHORT_CLICKED:
            ULOG_DEBUG("LV_EVENT_SHORT_CLICKED\n");
            lv_group_remove_all_objs(group);
            //lv_group_remove_obj(gui_screen.menu_screen);
            lv_group_add_obj(group, gui_screen.bat_percent);
            lv_group_add_obj(group, gui_screen.main_screen);
            lv_scr_load_anim(gui_screen.main_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 660, 0, false);
            break;
        case LV_EVENT_LONG_PRESSED:
            ULOG_DEBUG("menu_scr: LV_EVENT_LONG_PRESSED\n");
            break;

        default:
            break;

    }
}

void gui_menu_screen(void)
{
    LV_IMG_DECLARE(settings_icon);
    LV_IMG_DECLARE(statistics_icon);
    LV_IMG_DECLARE(battery_health_icon);

    /** Create menu screen */
    gui_screen.menu_screen = lv_obj_create(NULL, NULL);
    lv_obj_set_event_cb(gui_screen.menu_screen, event_menu_scr);   /*Assign an event callback*/
    lv_obj_add_style(gui_screen.menu_screen, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_screen);
    lv_scr_load_anim(gui_screen.menu_screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 660, 0, false);

    /** Create top bar with the style_top_bar */
    lv_obj_t * top_bar = lv_obj_create(gui_screen.menu_screen, NULL);
    lv_obj_set_size(top_bar, 160,20);
    lv_obj_add_style(top_bar, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_top_bar);
    lv_obj_align(top_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    /** Create top bar string */
    lv_obj_t * top_bar_str = lv_label_create(gui_screen.menu_screen, NULL);
    lv_label_set_text(top_bar_str, "Menu");
    lv_obj_set_style_local_text_color(top_bar_str, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);


//    gui_screen.icon_1 = lv_obj_create(gui_screen.menu_screen, NULL);
//    lv_obj_set_size(gui_screen.icon_1, 48,48);
//    lv_obj_add_style(gui_screen.icon_1, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_icon);
//    lv_obj_align(gui_screen.icon_1, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 1, -5);


    gui_screen.icon_mah = lv_img_create(gui_screen.menu_screen, NULL);
    lv_img_set_src(gui_screen.icon_mah, &settings_icon);
    lv_obj_align_origo(gui_screen.icon_mah, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 28, -31);

//
//    gui_screen.icon_2 = lv_obj_create(gui_screen.menu_screen, NULL);
//    lv_obj_set_size(gui_screen.icon_2, 48,48);
//    lv_obj_add_style(gui_screen.icon_2, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_icon);
//    lv_obj_align(gui_screen.icon_2, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 56, -5);


    gui_screen.icon_2 = lv_img_create(gui_screen.menu_screen, NULL);
    lv_img_set_src(gui_screen.icon_2, &statistics_icon);
    lv_obj_align_origo(gui_screen.icon_2, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 78, -31);

//    gui_screen.icon_3 = lv_obj_create(gui_screen.menu_screen, NULL);
//    lv_obj_set_size(gui_screen.icon_3, 48,48);
//    lv_obj_add_style(gui_screen.icon_3, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_icon);
//    lv_obj_align(gui_screen.icon_3, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 111, -5);

    gui_screen.icon_3 = lv_img_create(gui_screen.menu_screen, NULL);
    lv_img_set_src(gui_screen.icon_3, &battery_health_icon);
    lv_obj_align_origo(gui_screen.icon_3, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 127, -31);


    /** Create Gum-like icon */
    static lv_anim_path_t path_overshoot;
    lv_anim_path_init(&path_overshoot);
    lv_anim_path_set_cb(&path_overshoot, lv_anim_path_overshoot);

    static lv_anim_path_t path_ease_in_out;
    lv_anim_path_init(&path_ease_in_out);
    lv_anim_path_set_cb(&path_ease_in_out, lv_anim_path_ease_in_out);

    static lv_style_t style_gum;
    lv_style_init(&style_gum);
    //lv_style_set_border_color(&style_gum, LV_STATE_FOCUSED, LV_COLOR_WHITE);
    lv_style_set_border_width(&style_gum, LV_STATE_FOCUSED, 2);
    lv_style_set_transform_width(&style_gum, LV_STATE_PRESSED, 1);
    lv_style_set_transform_height(&style_gum, LV_STATE_PRESSED, -22);

    lv_style_set_transform_zoom(&style_gum, LV_STATE_FOCUSED, 350);
    lv_style_set_transform_zoom(&style_gum, LV_STATE_PRESSED, 10);

    lv_style_set_transform_width(&style_gum, LV_STATE_FOCUSED, 1);
    lv_style_set_transform_height(&style_gum, LV_STATE_FOCUSED, 1);

    lv_style_set_transform_width(&style_gum, LV_STATE_DEFAULT, -5);
    lv_style_set_transform_height(&style_gum, LV_STATE_DEFAULT, -5);

    //lv_style_set_transform_zoom(&style_gum, LV_STATE_FOCUSED, 50);

    lv_style_set_transition_path(&style_gum, LV_STATE_DEFAULT, &path_overshoot);
    lv_style_set_transition_path(&style_gum, LV_STATE_PRESSED, &path_ease_in_out);
    lv_style_set_transition_path(&style_gum, LV_STATE_FOCUSED, &path_ease_in_out);
    lv_style_set_transition_time(&style_gum, LV_STATE_DEFAULT, 250);
    lv_style_set_transition_delay(&style_gum, LV_STATE_DEFAULT, 100);
    lv_style_set_transition_prop_1(&style_gum, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_WIDTH);
    lv_style_set_transition_prop_2(&style_gum, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_HEIGHT);
    lv_style_set_transition_prop_3(&style_gum, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_ZOOM);


    //lv_obj_add_style(gui_screen.icon_1, LV_BTN_PART_MAIN, &style_gum);
    lv_obj_add_style(gui_screen.icon_2, LV_BTN_PART_MAIN, &style_gum);
    lv_obj_add_style(gui_screen.icon_3, LV_BTN_PART_MAIN, &style_gum);

    lv_obj_add_style(gui_screen.icon_mah, LV_BTN_PART_MAIN, &style_gum);



}