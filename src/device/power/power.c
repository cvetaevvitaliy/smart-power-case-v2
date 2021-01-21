#include "power.h"
#include "bq2589x_charger.h"
#include "usb_device.h"
#include "stm32_init.h"
#include "st7735s.h"
#include "cli.h"

Power_Status_t Power_status = {0};

bool Power_InitChargerChip(void)
{

    if (bq2589x_init_device() == true)
    {
        bq2589x_exit_ship_mode();
        bq2589x_exit_hiz_mode();
        bq2589x_set_charge_current(2000);
        bq2589x_adc_start(false);
        bq2589x_set_prechg_current(1024);
        bq2589x_set_bat_limit(2800);
        bq2589x_set_chargevoltage(MAX_VOLTAGE_LIION); // +1mV drop on wires
        bq2589x_set_term_current(TAPER_CURRENT);
        bq2589x_enable_max_charge(true);
        bq2589x_enable_charger();
        return true;
    }
    else
    {
        return false;
    }

}

void Power_LoopService(void)
{
    static uint32_t tv_delay = 0;
    static bool usb_enable = false;

    if (HAL_GetTick() - tv_delay > 500)
    {
        Power_status.vbat = bq2589x_adc_read_battery_volt();
        Power_status.vbus = bq2589x_adc_read_vbus_volt();
        Power_status.vbus_type = bq2589x_get_vbus_type();
        Power_status.charge_done = bq2589x_is_charge_done();
        Power_status.charging_status = (uint8_t)bq2589x_get_charging_status();
        ULOG_TRACE("Vbat: %d\n",Power_status.vbat );
        ULOG_TRACE("vbus: %d\n",Power_status.vbus );
        ULOG_TRACE("vbus_type: %d\n",Power_status.vbus_type );
        ULOG_TRACE("charge_done: %d\n",Power_status.charge_done );
        ULOG_TRACE("charging_status: %d\n",Power_status.charging_status );
        tv_delay = HAL_GetTick();
    }

    if (Power_status.vbus_type == BQ2589X_VBUS_USB_SDP  || Power_status.vbus_type == BQ2589X_VBUS_USB_CDP) {
        if (usb_enable == false) {
            usb_enable = true;
            USB_Reset();
            HAL_Delay(200);
            MX_USB_DEVICE_Init();
            LCD_ST7735S_Clear();
            LCD_ST7735S_drawRGBBitmap(0,0, &usb_to_pc);
            LCD_ST7735S_Update();
            HAL_Delay(2000);  /// need this time for reset and reinit USB
            cli_set_first_in_cli(false);
            LCD_ST7735S_Clear();

        }
    }
    if (Power_status.vbus_type == BQ2589X_VBUS_NONE && usb_enable == true) {
        MX_USB_DEVICE_DeInit();
        usb_enable = false;
        LCD_ST7735S_Clear();
        LCD_ST7735_DrawString("USB disconnected",15,35, &Font_8x10, ST7735_WHITE);
        LCD_ST7735S_Update();
        HAL_Delay(2000);
        cli_set_first_in_cli(true);
        LCD_ST7735S_Clear();
    }


}

Power_Status_t *Power_GetStatus(void)
{
    return &Power_status;
}
