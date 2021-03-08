#include "button.h"
#include "stm32f4xx.h"
#include "cli_log.h"


/** button is pressed or not*/
static bool button_is_pressed(uint8_t id)
{
    bool state = false;
    switch (id)
    {
        case 1:
            state = HAL_GPIO_ReadPin(BUTTON_1_GPIO_PORT, BUTTON_1_PIN);
            break;
        case 2:
            state = HAL_GPIO_ReadPin(BUTTON_2_GPIO_PORT, BUTTON_2_PIN);
            break;
        case 3:
            state = HAL_GPIO_ReadPin(BUTTON_3_GPIO_PORT, BUTTON_3_PIN);
            break;
        case 4:
            state = HAL_GPIO_ReadPin(BUTTON_4_GPIO_PORT, BUTTON_4_PIN);
            break;
        case 5:
            state = HAL_GPIO_ReadPin(BUTTON_5_GPIO_PORT, BUTTON_5_PIN);
            break;

        default:
            break;

    }

    return state;
}

/** Get the currently being pressed key.  0 if no key is pressed*/
static uint32_t keypad_get_key(void)
{
    for(uint8_t i = 1; i < 6; i++) {
        /*Return the pressed button's ID*/
        if(button_is_pressed(i)) {
            return i;
        }
    }

    return 0;
}

static char* lv_key_to_str(uint32_t key)
{
    static char *str;

    switch(key) {
        case LV_KEY_NEXT:
            str = "NEXT";
            break;
        case LV_KEY_PREV:
            str = "PREV";
            break;
        case LV_KEY_LEFT:
            str = "LEFT";
            break;
        case LV_KEY_RIGHT:
            str = "RIGHT";
            break;
        case LV_KEY_ENTER:
            str = "ENTER";
            break;
        case LV_KEY_UP:
            str = "UP";
            break;
        case LV_KEY_DOWN:
            str = "DOWN";
            break;

        default:
            str = "UNKNOWN";
            break;
    }

    return str;

}

bool keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint32_t last_key = 0;
    (void) indev_drv;      /*Unused*/

    /*Get whether the a key is pressed and save the pressed key*/
    uint32_t act_key = keypad_get_key();
    if(act_key != 0) {
        data->state = LV_INDEV_STATE_PR;

        /*Translate the keys to LVGL control characters according to your key definitions*/
        switch(act_key) {
            case 1:
                //act_key = LV_KEY_NEXT;
                act_key = LV_KEY_UP;
                break;
            case 2:
                //act_key = LV_KEY_PREV;
                act_key = LV_KEY_DOWN;
                break;
            case 3:
                //act_key = LV_KEY_LEFT;
                act_key = LV_KEY_PREV;
                break;
            case 4:
                //act_key = LV_KEY_RIGHT;
                act_key = LV_KEY_NEXT;
                break;
            case 5:
                act_key = LV_KEY_ENTER;
                break;

            default:
                break;
        }

        last_key = act_key;
        //ULOG_DEBUG("data->key: %s\n", lv_key_to_str(act_key));

    } else {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}



