#ifndef SMART_POWER_CASE_V2_GUI_SETTINGS_SCREEN_H
#define SMART_POWER_CASE_V2_GUI_SETTINGS_SCREEN_H
#include "lvgl.h"
#include "gui_style.h"

typedef struct {
    lv_obj_t * page;
    lv_obj_t * buzzer_icon;
    lv_obj_t * timer_icon;
    lv_obj_t * acc_icon;
    lv_obj_t * charger_icon;
    lv_obj_t * about_icon;
    lv_obj_t * exit_icon;

} gui_settings_scr_t;

void gui_settings_screen(void);

void gui_settings_screen_enable_focus(void);

void gui_settings_scr_set_actual_focus(lv_obj_t *obj);

#endif //SMART_POWER_CASE_V2_GUI_SETTINGS_SCREEN_H
