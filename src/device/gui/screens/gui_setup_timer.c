/**
 * @file gui_setup_timer.c
 * This is part of the project - smart-power-case-v2
 * Copyright (c) Vitaliy Nimych <vitaliy.nimych@gmail.com>
 * Created 02.05.2021
 * SPDX-License-Identifier: GNU General Public License v3.0
 * */

#include "gui_setup_timer.h"
#include "lvgl.h"
#include "ulog.h"
#include "gui.h"
#include "gui_settings_screen.h"
#include "eeprom.h"

extern gui_screen_t gui_screen;
extern gui_settings_scr_t gui_obj_settings_scr;
extern gui_style_t gui_style;
static lv_obj_t *top_bar_str = NULL;
static lv_obj_t *top_bar = NULL;

static lv_obj_t *spinbox;
static lv_obj_t *btn_inc;
static lv_obj_t *btn_dec;


static void lv_spinbox_increment_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if (event == LV_EVENT_FOCUSED) {
        lv_label_set_text(top_bar_str, "Increment time");
        lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
    }

    if(event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(spinbox);
    }
}

static void lv_spinbox_decrement_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if (event == LV_EVENT_FOCUSED) {
        lv_label_set_text(top_bar_str, "Decrement time");
        lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
    }

    if(event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(spinbox);
    }
}

static void event_setup_timer_scr(lv_obj_t * btn, lv_event_t event)
{
    static bool first_in = false;
    switch (event) {
        case LV_EVENT_FOCUSED: {

            if(first_in == false) {
                lv_label_set_text(top_bar_str, "Set timer auto-off (min)");
                first_in = true;
            }
            else
                lv_label_set_text(top_bar_str, "Save & Exit");

            lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);
        }
            break;

        case LV_EVENT_SHORT_CLICKED: {
            eepromData_t *eeprom_data = eeprom_GetSettings();
            eeprom_data->timerOff = (uint32_t) lv_spinbox_get_value(spinbox);
            eeprom_SaveSettings(eeprom_data);
            lv_group_remove_all_objs(gui_get_group_focus_obj());

            lv_group_t *set_group = gui_get_group_focus_obj();
            lv_group_add_obj(set_group, gui_obj_settings_scr.timer_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.acc_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.charger_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.about_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.exit_icon);
            lv_group_add_obj(set_group, gui_obj_settings_scr.buzzer_icon);
            lv_scr_load(gui_screen.settings_scr);
        }
            break;

        default:
            break;
    }

}

static void create_spinbox(void)
{
    spinbox = lv_spinbox_create(gui_screen.setup_timer_scr, NULL);
    lv_spinbox_set_range(spinbox, 0, 60);
    lv_spinbox_set_digit_format(spinbox, 2, 0);
    lv_spinbox_set_rollover(spinbox, false);
    lv_spinbox_set_padding_left(spinbox, 3);
    lv_obj_set_width(spinbox, 60);
    lv_obj_align(spinbox, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_text_font(spinbox, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_textarea_set_cursor_hidden(spinbox, true);

    lv_coord_t h = lv_obj_get_height(spinbox);

    btn_inc = lv_btn_create(gui_screen.setup_timer_scr, NULL);
    lv_obj_set_size(btn_inc, h, h);
    lv_obj_align(btn_inc, spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_theme_apply(btn_inc, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(btn_inc, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
    lv_obj_set_event_cb(btn_inc, lv_spinbox_increment_event_cb);

    btn_dec = lv_btn_create(gui_screen.setup_timer_scr, NULL);
    lv_obj_set_size(btn_dec, h, h);
    lv_obj_align(btn_dec, spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_theme_apply(btn_dec, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(btn_dec, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
    lv_obj_set_event_cb(btn_dec, lv_spinbox_decrement_event_cb);

    /** Applied Gum-like style for icons */
    lv_obj_add_style(btn_inc, LV_BTN_PART_MAIN, &gui_style.animation.style_spin);
    lv_obj_add_style(btn_dec, LV_BTN_PART_MAIN, &gui_style.animation.style_spin);

    /** Get eeprom data */
    eepromData_t *eeprom_data = eeprom_GetSettings();

    lv_spinbox_set_value(spinbox, eeprom_data->timerOff);

}

void gui_setup_timer(void)
{
    /** Create screen */
    gui_screen.setup_timer_scr = lv_obj_create(NULL, NULL);
    lv_obj_set_event_cb(gui_screen.setup_timer_scr, event_setup_timer_scr);   /*Assign an event callback*/
    lv_obj_add_style(gui_screen.setup_timer_scr, LV_OBJ_PART_MAIN, &gui_style.style_screen);

    /** Create top bar with the style_top_bar */
    top_bar = lv_obj_create(gui_screen.setup_timer_scr, NULL);
    lv_obj_set_size(top_bar, 160, 20);
    lv_obj_add_style(top_bar, LV_OBJ_PART_MAIN, &gui_style.style_top_bar);
    lv_obj_align(top_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    /** Create top bar string */
    top_bar_str = lv_label_create(gui_screen.setup_timer_scr, NULL);
    lv_obj_set_style_local_text_color(top_bar_str, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_align_origo(top_bar_str, top_bar, LV_ALIGN_CENTER, 0, 0);

    create_spinbox();

}

void gui_setup_timer_enable_focus(void)
{
    lv_group_t *set_group = gui_get_group_focus_obj();

    lv_group_add_obj(set_group, gui_screen.setup_timer_scr);

    lv_group_add_obj(set_group, btn_dec);
    lv_group_add_obj(set_group, btn_inc);

    /** Get eeprom data */
    eepromData_t *eeprom_data = eeprom_GetSettings();
    lv_spinbox_set_value(spinbox, eeprom_data->timerOff);

}
