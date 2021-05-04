#ifndef SMART_POWER_CASE_V2_GUI_H
#define SMART_POWER_CASE_V2_GUI_H
#include "lvgl.h"
#include "gui_style.h"

#define _unused __attribute__((unused))

typedef struct {
    /** Screens */
    lv_obj_t *main_scr;
    lv_obj_t *menu_scr;
    lv_obj_t *settings_scr;
    lv_obj_t *setup_buzzer_scr;
    lv_obj_t *setup_timer_scr;
    lv_obj_t *setup_acc_scr;

    /** Icons main screen */
    lv_obj_t *bat_percent;
    lv_obj_t *fill_icon_bat_big;
    lv_obj_t *icon_bat_big;
    lv_obj_t *button;

    /** Image icons menu screen */
    lv_obj_t *icon_1;
    lv_obj_t *icon_2;
    lv_obj_t *icon_3;

} gui_screen_t;

void gui_init(void);

lv_group_t* gui_get_group_focus_obj(void);

char* gui_debug_event(lv_event_t event);

#endif //SMART_POWER_CASE_V2_GUI_H
