#ifndef SMART_POWER_CASE_V2_GUI_STYLE_H
#define SMART_POWER_CASE_V2_GUI_STYLE_H
#include "lvgl.h"

typedef struct
{
    lv_style_t style_screen;
    lv_style_t style_top_bar;
    lv_style_t style_bot_bar;
    lv_style_t style_icon;
    lv_style_t style_text;
    lv_style_t style_big_bat;
    lv_style_t style_fill_bat;

} gui_style_t;

void gui_style_init(gui_style_t *style);

#endif //SMART_POWER_CASE_V2_GUI_STYLE_H
