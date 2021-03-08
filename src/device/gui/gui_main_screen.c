#include "gui_main_screen.h"
#include "gui.h"
#include "st7735s.h"
#include "stm32f4xx.h"
#include "tinyprintf.h"
#include "power.h"
#include "battery.h"
#include "cli.h"
#include "bq27441.h"
#include "lvgl.h"
#include "button.h"

gui_screen_t gui_screen = {0};

extern lv_group_t*  group;
static char bat_perc[100] = {0};

typedef enum {
    GUI_PRINT_VOLTAGE = 1,
    GUI_PRINT_PERCENT,
    GUI_PRINT_MAH,
    GUI_PRINT_TIME,
    GUI_PRINT_CURRENT,

    GUI_LAST_VALUE,
} gui_print_bat_st_t;

gui_print_bat_st_t gui_print_bat_st;

static void my_event_cb_scr_1(lv_obj_t * obj, lv_event_t event)
{
    ULOG_DEBUG("event %d\n", event);

    switch(event)
    {
        case LV_EVENT_FOCUSED:
            ULOG_DEBUG("LV_EVENT_FOCUSED\n");
            ULOG_DEBUG("Load screen 2\n");
            lv_scr_load_anim(gui_screen.main_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 660, 0, false);
            break;
        case LV_EVENT_SHORT_CLICKED:
            ULOG_DEBUG("LV_EVENT_SHORT_CLICKED\n");
            break;
        case LV_EVENT_LONG_PRESSED:
            ULOG_DEBUG("LV_EVENT_LONG_PRESSED\n");
            break;

        default:
            break;

    }
}

static void event_main_scr(lv_obj_t * obj, lv_event_t event)
{
    ULOG_DEBUG("event %d\n", event);

    switch(event)
    {
        case LV_EVENT_FOCUSED:
            ULOG_DEBUG("Main screen: FOCUSED, send event to bat_info\n");
            lv_event_send(gui_screen.bat_percent, LV_EVENT_FOCUSED, NULL);

            //lv_scr_load_anim(gui_screen.main_scr_1, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 660, 0, false);
            //lv_scr_load_anim(gui_screen.main_scr_2, LV_SCR_LOAD_ANIM_MOVE_TOP, 660, 0, false);
            break;
        case LV_EVENT_SHORT_CLICKED:
            lv_group_remove_all_objs(group);
            lv_group_add_obj(group, gui_screen.menu_screen);
            //lv_group_add_obj(group, gui_screen.icon_1);
            lv_group_add_obj(group, gui_screen.icon_mah);
            lv_group_add_obj(group, gui_screen.icon_2);
            lv_group_add_obj(group, gui_screen.icon_3);
            lv_scr_load_anim(gui_screen.menu_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 660, 0, false);
            ULOG_DEBUG("LV_EVENT_SHORT_CLICKED\n");
            break;
        case LV_EVENT_LONG_PRESSED:
            ULOG_DEBUG("LV_EVENT_LONG_PRESSED\n");
            break;

        default:
            break;

    }
}

static void event_handler_bat_info(lv_obj_t * obj, lv_event_t event)
{
    UNUSED(obj);

    ULOG_DEBUG("event_handler_bat_info: %d \n", event);

    switch(event)
    {
        case LV_EVENT_FOCUSED:
            ULOG_DEBUG("FOCUSED \n");
            gui_print_bat_st++;
            if (gui_print_bat_st > GUI_LAST_VALUE)
                gui_print_bat_st = GUI_PRINT_VOLTAGE;

            break;

        case LV_EVENT_SHORT_CLICKED:
            lv_event_send(gui_screen.main_screen, LV_EVENT_SHORT_CLICKED, NULL);
            break;
    }
}

void gui_main_screen(void)
{
    LV_IMG_DECLARE(battery_1);
    LV_IMG_DECLARE(battery_icon_big);
    LV_IMG_DECLARE(battery_icon_small);
    LV_IMG_DECLARE(mah_icon);
    LV_IMG_DECLARE(big_bat_tail_icon);

    gui_style_init(&gui_screen.gui_style_t);

    gui_screen.main_screen = lv_obj_create(NULL, NULL);
    lv_obj_set_event_cb(gui_screen.main_screen, event_main_scr);   /*Assign an event callback*/
    lv_obj_add_style(gui_screen.main_screen, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_screen);
    lv_scr_load_anim(gui_screen.main_screen, LV_SCR_LOAD_ANIM_NONE, 660, 0, false);


#if 0
    gui_screen.main_scr_1 = lv_obj_create(NULL, NULL);
    lv_obj_set_event_cb(gui_screen.main_scr_1, my_event_cb_scr_1);   /*Assign an event callback*/
    lv_scr_load_anim(gui_screen.main_scr_1, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, false);

    gui_screen.main_screen = lv_obj_create(NULL, NULL);
    lv_obj_set_event_cb(gui_screen.main_screen, my_event_cb_scr_2);   /*Assign an event callback*/

    lv_obj_add_style(gui_screen.main_scr_1, LV_OBJ_PART_MAIN, &style_screen);  //turn the screen white
    lv_obj_add_style(gui_screen.main_screen, LV_OBJ_PART_MAIN, &style_screen);  //turn the screen white

    /*Create 2 buttons*/
    lv_obj_t * btn1 = lv_btn_create(gui_screen.main_scr_1, NULL);         /*Create a button on the screen*/
    lv_obj_set_pos(btn1, 5, 5);              	   /*Set the position of the button*/
    //lv_obj_set_event_cb(btn1, my_event_cb);   /*Assign an event callback*/
    lv_obj_set_size(btn1, 65,22);

    lv_obj_t * btn2 = lv_btn_create(gui_screen.main_scr_2, NULL);         /*Copy the first button*/
    lv_obj_set_pos(btn2, 80, 5);                    /*Set the position of the button*/
    //lv_obj_set_event_cb(btn2, my_event_cb);   /*Assign an event callback*/
    lv_obj_set_size(btn2, 65,22);


    lv_obj_t * label1 = lv_label_create(btn1, NULL);	/*Create a label on the first button*/
    lv_label_set_text(label1, "Button 1");          	/*Set the text of the label*/

    lv_obj_t * label2 = lv_label_create(btn2, NULL);  	/*Create a label on the second button*/
    lv_label_set_text(label2, "Button 2");

    /*Create an object with the style_top_bar*/
    lv_obj_t * top_bar = lv_obj_create(gui_screen.main_scr_1, NULL);
    lv_obj_set_size(top_bar, 160,24);
    lv_obj_add_style(top_bar, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_top_bar);
    lv_obj_align(top_bar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_obj_t * bot_bar = lv_obj_create(gui_screen.main_scr_1, NULL);
    lv_obj_set_size(bot_bar, 160,80);
    lv_obj_add_style(bot_bar, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_top_bar);
    lv_obj_align(bot_bar, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    lv_obj_t * test_icon = lv_obj_create(gui_screen.main_scr_1, NULL);
    lv_obj_set_size(test_icon, 80,18);
    lv_obj_add_style(test_icon, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_icon);
    lv_obj_align(test_icon, NULL, LV_ALIGN_IN_TOP_LEFT, 3, 3);

    lv_obj_t * test_icon2 = lv_obj_create(gui_screen.main_scr_1, NULL);
    lv_obj_set_size(test_icon2, 80,18);
    lv_obj_add_style(test_icon2, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_icon);
    lv_obj_align(test_icon2, NULL, LV_ALIGN_IN_TOP_LEFT, 3, 24);

    lv_obj_t * test_icon3 = lv_obj_create(gui_screen.main_scr_1, NULL);
    lv_obj_set_size(test_icon3, 80,18);
    lv_obj_add_style(test_icon3, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_icon);
    lv_obj_align(test_icon3, NULL, LV_ALIGN_IN_TOP_LEFT, 3, 45);

    lv_obj_t * bat_icon_small;
    bat_icon_small = lv_img_create(gui_screen.main_scr_1, NULL);
    lv_img_set_src(bat_icon_small, &battery_icon_small);
    lv_obj_align_origo(bat_icon_small, NULL, LV_ALIGN_IN_TOP_LEFT, 136, 11);

    lv_obj_t * icon_mah;
    icon_mah = lv_img_create(test_icon, NULL);
    lv_img_set_src(icon_mah, &mah_icon);
    lv_obj_align_origo(icon_mah, test_icon, LV_ALIGN_IN_LEFT_MID, 15, 0);


    gui_screen.bat_percent = lv_label_create(bat_icon_small, NULL);
    lv_label_set_text(gui_screen.bat_percent, bat_perc);
    //lv_obj_add_style(gui_screen.bat_percent, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_top_bar);
    lv_obj_set_style_local_text_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_align_origo(gui_screen.bat_percent, NULL, LV_ALIGN_CENTER, -1, 0);
#endif

    gui_screen.icon_bat_big = lv_obj_create(gui_screen.main_screen, NULL);
    lv_obj_set_size(gui_screen.icon_bat_big, 108,56);
    lv_obj_add_style(gui_screen.icon_bat_big, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_big_bat);
    lv_obj_align(gui_screen.icon_bat_big, NULL, LV_ALIGN_CENTER, -6, 0);

    lv_obj_t * icon_bat_tail;
    icon_bat_tail = lv_img_create(gui_screen.main_screen, NULL);
    lv_img_set_src(icon_bat_tail, &big_bat_tail_icon);
    lv_obj_align_origo(icon_bat_tail, NULL, LV_ALIGN_IN_RIGHT_MID, -23, 0);

    gui_screen.fill_icon_bat_big = lv_obj_create(gui_screen.main_screen, NULL);
    lv_obj_set_size(gui_screen.fill_icon_bat_big, 100,48);
    lv_obj_add_style(gui_screen.fill_icon_bat_big, LV_OBJ_PART_MAIN, &gui_screen.gui_style_t.style_fill_bat);
    lv_obj_align(gui_screen.fill_icon_bat_big, NULL, LV_ALIGN_CENTER, -6, 0);


    gui_screen.bat_percent = lv_label_create(gui_screen.main_screen, NULL);
    lv_label_set_text(gui_screen.bat_percent, bat_perc);
    lv_obj_set_style_local_text_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_sel_bg_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_set_style_local_text_font(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_26);
    lv_obj_align_origo(gui_screen.bat_percent, gui_screen.fill_icon_bat_big, LV_ALIGN_CENTER, -49, -1);

    lv_label_set_long_mode(gui_screen.bat_percent, LV_LABEL_LONG_SROLL);     /*Circular scroll*/
    lv_obj_set_width(gui_screen.bat_percent, 98);
    lv_obj_set_style_local_text_letter_space(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 1);
    lv_obj_set_event_cb(gui_screen.bat_percent, event_handler_bat_info);


    lv_group_add_obj(group, gui_screen.bat_percent);
    lv_group_add_obj(group, gui_screen.main_screen);
    //lv_group_add_obj(group, gui_screen.main_scr_1);

}

void gui_set_bat_percent(uint8_t value)
{
    lv_obj_set_size(gui_screen.fill_icon_bat_big, value,48);
    if (value > 65 && value <= 100)
        lv_obj_set_style_local_bg_color(gui_screen.fill_icon_bat_big, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x20f962));
    else if (value > 20 && value <= 65)
        lv_obj_set_style_local_bg_color(gui_screen.fill_icon_bat_big, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xf5bb3d));
    else if (value > 0 && value <= 20)
        lv_obj_set_style_local_bg_color(gui_screen.fill_icon_bat_big, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);

//    sprintf(bat_perc, "%d%%,value);
//    lv_label_set_text(gui_screen.bat_percent, bat_perc);
//    if (value > 75 && value <= 100)
//        lv_obj_set_style_local_text_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
//    else if (value > 40 && value <= 75)
//        lv_obj_set_style_local_text_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
//    else
//        lv_obj_set_style_local_text_color(gui_screen.bat_percent, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

void gui_set_bat_voltage(float value)
{
//    sprintf(bat_perc, "%.2fV ", value);
//    lv_label_set_text(gui_screen.bat_percent, bat_perc);
//    lv_obj_align_origo(gui_screen.bat_percent, NULL, LV_ALIGN_CENTER, -1, 0);
}

void gui_update_value(void)
{
    Battery_status_t *battery_status = Battery_GetStatus();
    switch (gui_print_bat_st)
    {
        case GUI_PRINT_VOLTAGE:
            sprintf(bat_perc, "  %.2fV", battery_status->vbat);
            break;

        case GUI_PRINT_PERCENT:
        {
            if (battery_status->percent == 100)
                sprintf(bat_perc, "  %d%%", battery_status->percent);
            else if (battery_status->percent < 100 && battery_status->percent > 10)
                sprintf(bat_perc, "   %d%%", battery_status->percent);
            else
                sprintf(bat_perc, "    %d%%", battery_status->percent);
        }
            break;

        case GUI_PRINT_MAH:
            sprintf(bat_perc, "%dmAh", battery_status->capacity);
            break;

        case GUI_PRINT_CURRENT:
            sprintf(bat_perc, "%.3fA", (float) battery_status->current / 1000.f);
            break;

        case GUI_PRINT_TIME:
            sprintf(bat_perc, "1h22min");
            break;

        default:
            gui_print_bat_st = GUI_PRINT_VOLTAGE;
            break;
    }

    lv_label_set_text(gui_screen.bat_percent, bat_perc);
    gui_set_bat_percent(battery_status->percent);

}

