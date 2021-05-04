/**
 * @file gui_setup_acc.c
 * This is part of the project - smart-power-case-v2
 * Copyright (c) Vitaliy Nimych <vitaliy.nimych@gmail.com>
 * Created 02.05.2021
 * SPDX-License-Identifier: GNU General Public License v3.0
 * */

#include "gui_setup_acc.h"
#include "lvgl.h"
#include "ulog.h"
#include "gui.h"
#include "gui_settings_screen.h"
#include "eeprom.h"

extern gui_screen_t gui_screen;
extern gui_style_t gui_style;
static lv_obj_t* top_bar_str = NULL;
static lv_obj_t* top_bar = NULL;
static lv_obj_t* checkbox;
static lv_obj_t* accept_ico;
static lv_obj_t* cancel_ico;
extern gui_settings_scr_t gui_obj_settings_scr;

#define ENABLE              "Enable double click"
#define DISABLE             "Disable double click"

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        ULOG_DEBUG("State: %s\n", lv_checkbox_is_checked(obj) ? "Checked" : "Unchecked");
        if (lv_checkbox_is_checked(obj))
            lv_checkbox_set_text(checkbox, ENABLE);
        else
            lv_checkbox_set_text(checkbox, DISABLE);
        lv_obj_align(checkbox, NULL, LV_ALIGN_CENTER, 0, -5);
    }
    if (event == LV_EVENT_FOCUSED) {
        lv_label_set_text(top_bar_str, "Setup accelerometer");
        lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
    }

}

static void accept_event_handler(lv_obj_t * obj, lv_event_t event)
{
    switch (event) {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Save");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED: {
            eepromData_t *eeprom_data = eeprom_GetSettings();
            eeprom_data->accState = (uint32_t) lv_checkbox_is_checked(checkbox);
            eeprom_SaveSettings(eeprom_data);

            lv_group_remove_all_objs(gui_get_group_focus_obj());
            lv_group_t *set_group = gui_get_group_focus_obj();
            lv_group_add_obj(set_group, gui_obj_settings_scr.acc_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.charger_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.about_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.exit_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.buzzer_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.timer_icon);
            lv_scr_load(gui_screen.settings_scr);
        }
            break;

        default:
            break;
    }
}

static void cancel_event_handler(lv_obj_t * obj, lv_event_t event)
{
    switch (event) {
        case LV_EVENT_FOCUSED:
            lv_label_set_text(top_bar_str, "Cancel");
            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
            break;

        case LV_EVENT_SHORT_CLICKED:
            lv_group_remove_all_objs(gui_get_group_focus_obj());
            gui_settings_screen_enable_focus();
            lv_scr_load(gui_screen.settings_scr);
            break;

        default:
            break;
    }
}

void gui_setup_acc(void)
{

    /** Get eeprom data */
    eepromData_t *eeprom_data = eeprom_GetSettings();

    /** Declare images for icons  */
    LV_IMG_DECLARE(accept_icon);
    LV_IMG_DECLARE(cancel_icon);

    /** Create screen */
    gui_screen.setup_acc_scr = lv_obj_create(NULL, NULL);
    //lv_obj_set_event_cb(gui_screen.setup_buzzer_scr, event_settings_scr);
    lv_obj_add_style(gui_screen.setup_acc_scr, LV_OBJ_PART_MAIN, &gui_style.style_screen);

    /** Create top bar with the style_top_bar */
    top_bar = lv_obj_create(gui_screen.setup_acc_scr, NULL);
    lv_obj_set_size(top_bar, 160, 20);
    lv_obj_add_style(top_bar, LV_OBJ_PART_MAIN, &gui_style.style_top_bar);
    lv_obj_align(top_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    /** Create top bar string */
    top_bar_str = lv_label_create(gui_screen.setup_acc_scr, NULL);
    lv_label_set_text(top_bar_str, "Setup accelerometer");
    lv_obj_set_style_local_text_color(top_bar_str, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);

    checkbox = lv_checkbox_create(gui_screen.setup_acc_scr, NULL);
    if (eeprom_data->accState) {
        lv_checkbox_set_checked(checkbox, true);
        lv_checkbox_set_text(checkbox, ENABLE);
    }
    else {
        lv_checkbox_set_checked(checkbox, false);
        lv_checkbox_set_text(checkbox, DISABLE);
    }

    lv_obj_align(checkbox, NULL, LV_ALIGN_CENTER, 0, -5);
    lv_obj_set_event_cb(checkbox, event_handler);

    /** Create Accept icon */
    accept_ico = lv_img_create(gui_screen.setup_acc_scr, NULL);
    lv_img_set_src(accept_ico, &accept_icon);
    lv_obj_align(accept_ico, NULL, LV_ALIGN_CENTER, -17, 23);
    lv_obj_set_event_cb(accept_ico, accept_event_handler);

    /** Create Cancel icon */
    cancel_ico = lv_img_create(gui_screen.setup_acc_scr, NULL);
    lv_img_set_src(cancel_ico, &cancel_icon);
    lv_obj_align(cancel_ico, NULL, LV_ALIGN_CENTER, 15, 23);
    lv_obj_set_event_cb(cancel_ico, cancel_event_handler);

    /** Applied Gum-like style for icons */
    lv_obj_add_style(accept_ico, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);
    lv_obj_add_style(cancel_ico, LV_BTN_PART_MAIN, &gui_style.animation.style_gum);

}

void gui_setup_acc_enable_focus(void)
{
    lv_group_t *set_group = gui_get_group_focus_obj();

    lv_group_add_obj(set_group, checkbox);
    lv_group_add_obj(set_group, accept_ico);
    lv_group_add_obj(set_group, cancel_ico);

}
