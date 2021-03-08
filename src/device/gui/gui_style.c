#include "gui_style.h"


void gui_style_init(gui_style_t *style)
{
    /*****************         Init style for all screen      **************/
    lv_style_init(&style->style_screen);
    lv_style_set_bg_color(&style->style_screen, LV_STATE_DEFAULT, LV_COLOR_BLACK);


    /*****************         Init style for TOP bar         **************/
    /** Set a background color and a radius */
    lv_style_set_radius(&style->style_top_bar, LV_STATE_DEFAULT, 6);
    lv_style_set_bg_opa(&style->style_top_bar, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style->style_top_bar, LV_STATE_DEFAULT, LV_COLOR_PURPLE);

    /** Add border to the bottom+right */
    lv_style_set_border_color(&style->style_top_bar, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&style->style_top_bar, LV_STATE_DEFAULT, 1);
    lv_style_set_border_opa(&style->style_top_bar, LV_STATE_DEFAULT, LV_OPA_100);
    //lv_style_set_border_side(&style->style_top_bar, LV_STATE_DEFAULT, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);

    /** Set color for text */
    //lv_style_set_text_color(&style->style_top_bar, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    //lv_style_set_value_color(&style->style_top_bar, LV_STATE_DEFAULT, LV_COLOR_WHITE);


    /*****************         Init style for ICON          **************/
    /** Set a background color and a radius */
    lv_style_set_radius(&style->style_icon, LV_STATE_DEFAULT, 4);
    lv_style_set_bg_opa(&style->style_icon, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style->style_icon, LV_STATE_DEFAULT, LV_COLOR_BLACK); //lv_color_hex(0x000cff)); // BLUE

    /** Add border to the bottom+right */
    lv_style_set_border_color(&style->style_icon, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&style->style_icon, LV_STATE_DEFAULT, 1);
    lv_style_set_border_opa(&style->style_icon, LV_STATE_DEFAULT, LV_OPA_100);
    //lv_style_set_border_side(&style->style_icon, LV_STATE_DEFAULT, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);



    /*****************         Init style for BOTTOM bar         **************/
    /** Set a background color and a radius */
    lv_style_set_radius(&style->style_bot_bar, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_opa(&style->style_bot_bar, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style->style_bot_bar, LV_STATE_DEFAULT, LV_COLOR_PURPLE);

    /** Add border to the bottom+right */
    lv_style_set_border_color(&style->style_bot_bar, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&style->style_bot_bar, LV_STATE_DEFAULT, 1);
    lv_style_set_border_opa(&style->style_bot_bar, LV_STATE_DEFAULT, LV_OPA_100);
    //lv_style_set_border_side(&style->style_bot_bar, LV_STATE_DEFAULT, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);



    /*****************         Init style for big bat        **************/
    /** Set a background color and a radius */
    lv_style_set_radius(&style->style_big_bat, LV_STATE_DEFAULT, 11);
    lv_style_set_bg_opa(&style->style_big_bat, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style->style_big_bat, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    /** Add border to the bottom+right */
    lv_style_set_border_color(&style->style_big_bat, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&style->style_big_bat, LV_STATE_DEFAULT, 3);
    lv_style_set_border_opa(&style->style_big_bat, LV_STATE_DEFAULT, LV_OPA_100);
    //lv_style_set_border_side(&style->style_icon, LV_STATE_DEFAULT, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);


    /*****************         Init style for fill big bat        **************/
    /** Set a background color and a radius */
    lv_style_set_radius(&style->style_fill_bat, LV_STATE_DEFAULT, 7);
    lv_style_set_bg_opa(&style->style_fill_bat, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style->style_fill_bat, LV_STATE_DEFAULT, lv_color_hex(0x20f962));

    /** Add border to the bottom+right */
    lv_style_set_border_color(&style->style_fill_bat, LV_STATE_DEFAULT, lv_color_hex(0x20f962));
    lv_style_set_border_width(&style->style_fill_bat, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&style->style_fill_bat, LV_STATE_DEFAULT, LV_OPA_100);
    //lv_style_set_border_side(&style->style_icon, LV_STATE_DEFAULT, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);


}