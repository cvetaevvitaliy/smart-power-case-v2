#ifndef SMART_POWER_CASE_V2_GUI_MAIN_SCREEN_H
#define SMART_POWER_CASE_V2_GUI_MAIN_SCREEN_H
#include "lvgl.h"
#include "gui_style.h"

typedef struct {
    lv_obj_t * main_screen;
    lv_obj_t * bat_percent;
    lv_obj_t * fill_icon_bat_big;
    lv_obj_t * icon_bat_big;
    gui_style_t gui_style_t;

} gui_screen_t;

void gui_main_screen_1(void);

void gui_set_bat_percent(uint8_t value);
void gui_set_bat_voltage(float value);

#endif //SMART_POWER_CASE_V2_GUI_MAIN_SCREEN_H
