#ifndef SMART_POWER_CASE_V2_GUI_STYLE_H
#define SMART_POWER_CASE_V2_GUI_STYLE_H
#include "lvgl.h"


typedef struct {

    lv_anim_path_t path_ease_in_out;
    lv_anim_path_t path_overshoot;
    lv_style_t style_gum;
    lv_style_t style_spin;

} gui_animation_t;

typedef struct
{
    lv_style_t style_screen;
    lv_style_t style_page;
    lv_style_t style_top_bar;
    lv_style_t style_bot_bar;
    lv_style_t style_window;
    lv_style_t style_icon;
    lv_style_t style_text;
    lv_style_t style_big_bat;
    lv_style_t style_fill_bat;
    gui_animation_t animation;

} gui_style_t;

void gui_style_init(gui_style_t *style);

#endif //SMART_POWER_CASE_V2_GUI_STYLE_H
