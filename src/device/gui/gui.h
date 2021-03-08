#ifndef SMART_POWER_CASE_V2_GUI_H
#define SMART_POWER_CASE_V2_GUI_H
#include "lvgl.h"
#include "gui_style.h"

typedef struct {
    lv_obj_t * main_screen;
    lv_obj_t * menu_screen;
    lv_obj_t * bat_percent;
    lv_obj_t * fill_icon_bat_big;
    lv_obj_t * icon_bat_big;
    lv_obj_t * button;
    gui_style_t gui_style_t;

    lv_obj_t * icon_1;
    lv_obj_t * icon_2;
    lv_obj_t * icon_3;
    lv_obj_t * icon_mah;

} gui_screen_t;

void gui_init(void);

#endif //SMART_POWER_CASE_V2_GUI_H
