#ifndef SMART_POWER_CASE_V2_GUI_MAIN_SCREEN_H
#define SMART_POWER_CASE_V2_GUI_MAIN_SCREEN_H
#include "lvgl.h"
#include "gui_style.h"

void gui_main_screen(void);

void gui_set_bat_percent(uint8_t value);
void gui_set_bat_voltage(float value);

void gui_update_value(void);

#endif //SMART_POWER_CASE_V2_GUI_MAIN_SCREEN_H
