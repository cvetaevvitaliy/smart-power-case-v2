#include "gui_style.h"

static void gum_like_animation_init(gui_style_t *style);
static void spin_like_animation_init(gui_style_t *style);

void gui_style_init(gui_style_t *style)
{
    gum_like_animation_init(style);
    spin_like_animation_init(style);

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


    /*****************           Init style for Window          **************/
    //lv_style_init(&style->style_window);
    lv_style_set_bg_color(&style->style_window, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_color(&style->style_window, LV_STATE_DEFAULT, LV_COLOR_WHITE);


    lv_style_set_radius(&style->style_window, LV_STATE_DEFAULT, 1);
    lv_style_set_bg_opa(&style->style_window, LV_STATE_DEFAULT, LV_OPA_COVER);

    lv_style_set_border_color(&style->style_window, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&style->style_window, LV_STATE_DEFAULT, 1);
    lv_style_set_border_opa(&style->style_window, LV_STATE_DEFAULT, LV_OPA_100);

    /*****************           Init style for Pages            **************/
    //lv_style_init(&style->style_window);
    lv_style_set_bg_color(&style->style_page, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_color(&style->style_page, LV_STATE_DEFAULT, LV_COLOR_WHITE);


    lv_style_set_radius(&style->style_page, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_opa(&style->style_page, LV_STATE_DEFAULT, LV_OPA_COVER);

    lv_style_set_border_color(&style->style_page, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&style->style_page, LV_STATE_DEFAULT, 0);
    lv_style_set_border_opa(&style->style_page, LV_STATE_DEFAULT, LV_OPA_TRANSP);


}


void gum_like_animation_init(gui_style_t *style)
{

    /** Create Gum-like icon */
    lv_anim_path_init(&style->animation.path_overshoot);
    lv_anim_path_set_cb(&style->animation.path_overshoot, lv_anim_path_overshoot);

    lv_anim_path_init(&style->animation.path_ease_in_out);
    lv_anim_path_set_cb(&style->animation.path_ease_in_out, lv_anim_path_ease_in_out);

    lv_style_init(&style->animation.style_gum);

    lv_style_set_border_width(&style->animation.style_gum, LV_STATE_FOCUSED, 2);
    lv_style_set_transform_width(&style->animation.style_gum, LV_STATE_PRESSED, 1);
    lv_style_set_transform_height(&style->animation.style_gum, LV_STATE_PRESSED, -22);

    lv_style_set_transform_zoom(&style->animation.style_gum, LV_STATE_FOCUSED, 310);
    lv_style_set_transform_zoom(&style->animation.style_gum, LV_STATE_PRESSED, 10);

    lv_style_set_transform_width(&style->animation.style_gum, LV_STATE_FOCUSED, 1);
    lv_style_set_transform_height(&style->animation.style_gum, LV_STATE_FOCUSED, 1);

    lv_style_set_transform_width(&style->animation.style_gum, LV_STATE_DEFAULT, -5);
    lv_style_set_transform_height(&style->animation.style_gum, LV_STATE_DEFAULT, -5);

    lv_style_set_transition_path(&style->animation.style_gum, LV_STATE_DEFAULT, &style->animation.path_overshoot);
    lv_style_set_transition_path(&style->animation.style_gum, LV_STATE_PRESSED, &style->animation.path_ease_in_out);
    lv_style_set_transition_path(&style->animation.style_gum, LV_STATE_FOCUSED, &style->animation.path_ease_in_out);
    lv_style_set_transition_time(&style->animation.style_gum, LV_STATE_DEFAULT, 250);
    lv_style_set_transition_delay(&style->animation.style_gum, LV_STATE_DEFAULT, 100);
    lv_style_set_transition_prop_1(&style->animation.style_gum, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_WIDTH);
    lv_style_set_transition_prop_2(&style->animation.style_gum, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_HEIGHT);
    lv_style_set_transition_prop_3(&style->animation.style_gum, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_ZOOM);

}

void spin_like_animation_init(gui_style_t *style)
{

    /** Create Gum-like icon */
    lv_anim_path_init(&style->animation.path_overshoot);
    lv_anim_path_set_cb(&style->animation.path_overshoot, lv_anim_path_overshoot);

    lv_anim_path_init(&style->animation.path_ease_in_out);
    lv_anim_path_set_cb(&style->animation.path_ease_in_out, lv_anim_path_ease_in_out);

    lv_style_init(&style->animation.style_spin);

    lv_style_set_border_width(&style->animation.style_spin, LV_STATE_FOCUSED, 2);
    lv_style_set_radius(&style->animation.style_spin, LV_STATE_FOCUSED, 4);
    lv_style_set_transform_width(&style->animation.style_spin, LV_STATE_PRESSED, 1);
    lv_style_set_transform_height(&style->animation.style_spin, LV_STATE_PRESSED, -5);

    lv_style_set_transform_zoom(&style->animation.style_spin, LV_STATE_FOCUSED, 310);
    lv_style_set_transform_zoom(&style->animation.style_spin, LV_STATE_PRESSED, 10);

    lv_style_set_transform_width(&style->animation.style_spin, LV_STATE_FOCUSED, 1);
    lv_style_set_transform_height(&style->animation.style_spin, LV_STATE_FOCUSED, 1);

    lv_style_set_transform_width(&style->animation.style_spin, LV_STATE_DEFAULT, -5);
    lv_style_set_transform_height(&style->animation.style_spin, LV_STATE_DEFAULT, -5);

    lv_style_set_transition_path(&style->animation.style_spin, LV_STATE_DEFAULT, &style->animation.path_overshoot);
    lv_style_set_transition_path(&style->animation.style_spin, LV_STATE_PRESSED, &style->animation.path_ease_in_out);
    lv_style_set_transition_path(&style->animation.style_spin, LV_STATE_FOCUSED, &style->animation.path_ease_in_out);
    lv_style_set_transition_time(&style->animation.style_spin, LV_STATE_DEFAULT, 250);
    lv_style_set_transition_delay(&style->animation.style_spin, LV_STATE_DEFAULT, 100);
    lv_style_set_transition_prop_1(&style->animation.style_spin, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_WIDTH);
    lv_style_set_transition_prop_2(&style->animation.style_spin, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_HEIGHT);
    lv_style_set_transition_prop_3(&style->animation.style_spin, LV_STATE_DEFAULT, LV_STYLE_TRANSFORM_ZOOM);

    //lv_style_set_bg_color(&style->animation.style_spin, LV_STATE_DEFAULT, LV_COLOR_PURPLE);
    lv_style_set_border_color(&style->animation.style_spin, LV_STATE_FOCUSED, LV_COLOR_PURPLE);
    lv_style_set_border_color(&style->animation.style_spin, LV_STATE_PRESSED, LV_COLOR_PURPLE);
    lv_style_set_border_color(&style->animation.style_spin, LV_STATE_CHECKED, LV_COLOR_PURPLE);
    lv_style_set_border_color(&style->animation.style_spin, LV_STATE_DEFAULT, LV_COLOR_WHITE);

}