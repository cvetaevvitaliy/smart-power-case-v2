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
#include "gui_setup_buzzer.h"
#include "gui_setup_timer.h"
#include "gui_setup_acc.h"

gui_settings_scr_t gui_obj_settings_scr = {0};
extern gui_screen_t gui_screen;
extern gui_style_t gui_style;
static lv_obj_t* top_bar_str = NULL;
static lv_obj_t* top_bar = NULL;


static void event_handler_buzzer_icon(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__, gui_debug_event(event));
    switch (event) {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Setup buzzer");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            /** Remove all object in focus joystick and add new objects for focus */
            lv_group_remove_all_objs(gui_get_group_focus_obj());
            gui_setup_buzzer_enable_focus();
            lv_scr_load(gui_screen.setup_buzzer_scr);
            break;

        default:
            break;
    }
}

static void event_handler_timer_icon(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__, gui_debug_event(event));
    switch (event) {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Setup timer");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            /** Remove all object in focus joystick and add new objects for focus */
            lv_group_remove_all_objs(gui_get_group_focus_obj());
            gui_setup_timer_enable_focus();
            lv_scr_load(gui_screen.setup_timer_scr);
            break;

        default:
            break;
    }
}

static void event_handler_acc_icon(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__, gui_debug_event(event));
    switch (event) {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Setup accelerometer");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            /** Remove all object in focus joystick and add new objects for focus */
            lv_group_remove_all_objs(gui_get_group_focus_obj());
            gui_setup_acc_enable_focus();
            lv_scr_load(gui_screen.setup_acc_scr);
            break;

        default:
            break;
    }
}

static void event_handler_charger_icon(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__, gui_debug_event(event));
    switch (event) {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Setup charger");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            break;

        default:
            break;
    }
}

static void event_handler_about_icon(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__, gui_debug_event(event));
    switch (event) {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "About");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            break;

        default:
            break;
    }
}

static void event_handler_exit_icon(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);
    ULOG_DEBUG("%s: %s\n", __FUNCTION__, gui_debug_event(event));
    switch (event) {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Exit");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:

            /** Remove all object in focus joystick and add new objects for focus */
            lv_group_remove_all_objs(gui_get_group_focus_obj());
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.menu_scr);
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.icon_1);
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.icon_2);
            lv_group_add_obj(gui_get_group_focus_obj(), gui_screen.icon_3);
            lv_scr_load_anim(gui_screen.menu_scr, LV_SCR_LOAD_ANIM_OVER_LEFT, 500, 100, false);
            break;

        default:
            break;
    }
}

void gui_settings_screen(void)
{
    /** Declare images for icons  */
    LV_IMG_DECLARE(buzzer_icon);
    LV_IMG_DECLARE(timer_icon);
    LV_IMG_DECLARE(acc_icon);
    LV_IMG_DECLARE(charg_icon);
    LV_IMG_DECLARE(about_icon);
    LV_IMG_DECLARE(exit_icon);
    LV_IMG_DECLARE(info_icon);

    /** Create menu screen */
    gui_screen.settings_scr = lv_obj_create(NULL, NULL);
    //lv_obj_set_event_cb(gui_screen.settings_screen, event_settings_scr);   /*Assign an event callback*/
    lv_obj_add_style(gui_screen.settings_scr, LV_OBJ_PART_MAIN, &gui_style.style_screen);

    /*Create a page*/
    gui_obj_settings_scr.page = lv_page_create(gui_screen.settings_scr, NULL);
    lv_obj_set_size(gui_obj_settings_scr.page, 160, 60);
    lv_obj_align(gui_obj_settings_scr.page, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_obj_add_style(gui_obj_settings_scr.page, LV_OBJ_PART_MAIN, &gui_style.style_page);

    /** Create top bar with the style_top_bar */
    top_bar = lv_obj_create(gui_screen.settings_scr, NULL);
    lv_obj_set_size(top_bar, 160, 20);
    lv_obj_add_style(top_bar, LV_OBJ_PART_MAIN, &gui_style.style_top_bar);
    lv_obj_align(top_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    /** Create top bar string */
    top_bar_str = lv_label_create(gui_screen.settings_scr, NULL);
    lv_label_set_text(top_bar_str, "Settings");
    lv_obj_set_style_local_text_color(top_bar_str, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);

    /** Create Buzzer icon */
    gui_obj_settings_scr.buzzer_icon = lv_img_create(gui_obj_settings_scr.page, NULL);
    lv_img_set_src(gui_obj_settings_scr.buzzer_icon, &buzzer_icon);
    lv_obj_align_origo(gui_obj_settings_scr.buzzer_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 22, -23);

    /** Create Timer icon */
    gui_obj_settings_scr.timer_icon = lv_img_create(gui_obj_settings_scr.page, NULL);
    lv_img_set_src(gui_obj_settings_scr.timer_icon, &timer_icon);
    lv_obj_align_origo(gui_obj_settings_scr.timer_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 72, -23);

    /** Create Acc icon */
    gui_obj_settings_scr.acc_icon = lv_img_create(gui_obj_settings_scr.page, NULL);
    lv_img_set_src(gui_obj_settings_scr.acc_icon, &acc_icon);
    lv_obj_align_origo(gui_obj_settings_scr.acc_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 122, -23);

    /** Create Charger icon */
    gui_obj_settings_scr.charger_icon = lv_img_create(gui_obj_settings_scr.page, NULL);
    lv_img_set_src(gui_obj_settings_scr.charger_icon, &charg_icon);
    lv_obj_align_origo(gui_obj_settings_scr.charger_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 172, -23);

    /** Create About icon */
    gui_obj_settings_scr.about_icon = lv_img_create(gui_obj_settings_scr.page, NULL);
    lv_img_set_src(gui_obj_settings_scr.about_icon, &info_icon);
    lv_obj_align_origo(gui_obj_settings_scr.about_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 222, -23);

    /** Create Exit icon */
    gui_obj_settings_scr.exit_icon = lv_img_create(gui_obj_settings_scr.page, NULL);
    lv_img_set_src(gui_obj_settings_scr.exit_icon, &exit_icon);
    lv_obj_align_origo(gui_obj_settings_scr.exit_icon, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 272, -23);

    /** Applied Gum-like style for icons in menu */
    lv_obj_add_style(gui_obj_settings_scr.buzzer_icon, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);
    lv_obj_add_style(gui_obj_settings_scr.timer_icon, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);
    lv_obj_add_style(gui_obj_settings_scr.acc_icon, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);
    lv_obj_add_style(gui_obj_settings_scr.charger_icon, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);
    lv_obj_add_style(gui_obj_settings_scr.about_icon, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);
    lv_obj_add_style(gui_obj_settings_scr.exit_icon, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);

    /** Register event callbacks */
    lv_obj_set_event_cb(gui_obj_settings_scr.buzzer_icon, event_handler_buzzer_icon);
    lv_obj_set_event_cb(gui_obj_settings_scr.timer_icon, event_handler_timer_icon);
    lv_obj_set_event_cb(gui_obj_settings_scr.acc_icon, event_handler_acc_icon);
    lv_obj_set_event_cb(gui_obj_settings_scr.charger_icon, event_handler_charger_icon);
    lv_obj_set_event_cb(gui_obj_settings_scr.about_icon, event_handler_about_icon);
    lv_obj_set_event_cb(gui_obj_settings_scr.exit_icon, event_handler_exit_icon);


}


void gui_settings_screen_enable_focus(void)
{
    lv_group_t *set_group = gui_get_group_focus_obj();

    //lv_group_add_obj(set_group, gui_screen.settings_screen);
    lv_group_add_obj(set_group, gui_obj_settings_scr.buzzer_icon);
    lv_group_add_obj(set_group, gui_obj_settings_scr.timer_icon);
    lv_group_add_obj(set_group, gui_obj_settings_scr.acc_icon);
    lv_group_add_obj(set_group, gui_obj_settings_scr.charger_icon);
    lv_group_add_obj(set_group, gui_obj_settings_scr.about_icon);
    lv_group_add_obj(set_group, gui_obj_settings_scr.exit_icon);

    lv_page_focus(gui_obj_settings_scr.page, gui_obj_settings_scr.buzzer_icon, LV_ANIM_OFF);

}

void gui_settings_scr_set_actual_focus(lv_obj_t *obj)
{

    if(obj == gui_obj_settings_scr.buzzer_icon) {
        lv_page_focus(gui_obj_settings_scr.page, obj, LV_ANIM_ON);
    }

    if(obj == gui_obj_settings_scr.timer_icon) {
        lv_page_focus(gui_obj_settings_scr.page, obj, LV_ANIM_ON);
    }

    if(obj == gui_obj_settings_scr.acc_icon) {
        lv_page_focus(gui_obj_settings_scr.page, obj, LV_ANIM_ON);
    }

    if(obj == gui_obj_settings_scr.charger_icon) {
        lv_page_focus(gui_obj_settings_scr.page, obj, LV_ANIM_ON);
    }

    if(obj == gui_obj_settings_scr.about_icon) {
        lv_page_focus(gui_obj_settings_scr.page, obj, LV_ANIM_ON);
    }

    if(obj == gui_obj_settings_scr.exit_icon) {
        lv_page_focus(gui_obj_settings_scr.page, obj, LV_ANIM_ON);
    }

}
