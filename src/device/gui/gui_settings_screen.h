#ifndef SMART_POWER_CASE_V2_GUI_SETTINGS_SCREEN_H
#define SMART_POWER_CASE_V2_GUI_SETTINGS_SCREEN_H
#include "lvgl.h"
#include "gui_style.h"

typedef struct {
    lv_obj_t * buzzer_icon;

} gui_settings_scr_t;

void gui_settings_screen(void);

void gui_settings_screen_enable_focus(void);

#endif //SMART_POWER_CASE_V2_GUI_SETTINGS_SCREEN_H
