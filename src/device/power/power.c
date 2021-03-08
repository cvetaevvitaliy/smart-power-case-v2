#include "power.h"
#include "bq2589x_charger.h"
#include "usb_device.h"
#include "stm32_init.h"
#include "st7735s.h"
#include "cli.h"
#include "cmsis_os.h"

extern I2C_HandleTypeDef hi2c1;
static Power_Status_t Power_status = {0};

static int16_t _read_byte(uint8_t DevAddress, uint8_t *data, uint8_t reg)
{

  if(HAL_I2C_Mem_Read(&hi2c1, (uint16_t)DevAddress, reg,1, data,1,25)==HAL_OK)
  {
    return BQ25895_OK;
  }
  else
  {
    return BQ25895_ERR;
  }

}

static int16_t _write_byte(uint8_t DevAddress, uint8_t reg, uint8_t *data)
{

  if(HAL_I2C_Mem_Write(&hi2c1, (uint16_t)DevAddress, reg, 1, data, 1, 25) == HAL_OK)
  {
    return BQ25895_OK;
  }
  else
  {
    return BQ25895_ERR;
  }

}

bq2589x_ctx_t bq2589x =
        {
            .write_reg = _write_byte,
            .read_reg = _read_byte,
            .bq25895x_i2c_address = BQ25895_ADDR,
        };

bool Power_InitChargerChip(void)
{

    if (bq2589x_init_device(&bq2589x) == BQ25895_OK)
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

void Power_Loop(void)
{
    static uint32_t tv_delay = 0;
    static bool usb_enable = false;

    Power_status.vbat = bq2589x_adc_read_battery_volt();
    Power_status.vbus = bq2589x_adc_read_vbus_volt();
    Power_status.vbus_type = bq2589x_get_vbus_type();
    Power_status.charge_done = bq2589x_is_charge_done();
    Power_status.charging_status = (uint8_t) bq2589x_get_charging_status();
    ULOG_TRACE("Vbat: %d\n", Power_status.vbat);
    ULOG_TRACE("vbus: %d\n", Power_status.vbus);
    ULOG_TRACE("vbus_type: %d\n", Power_status.vbus_type);
    ULOG_TRACE("charge_done: %d\n", Power_status.charge_done);
    ULOG_TRACE("charging_status: %d\n", Power_status.charging_status);
    tv_delay = HAL_GetTick();

    if (Power_status.vbus_type == BQ2589X_VBUS_USB_SDP || Power_status.vbus_type == BQ2589X_VBUS_USB_CDP)
    {
        if (usb_enable == false)
        {
            usb_enable = true;
            USB_Reset();
            HAL_Delay(200);
            MX_USB_DEVICE_Init();
            //LCD_ST7735S_Clear();
            //LCD_ST7735S_Draw_RGB_Bitmap(0, 0, &usb_to_pc);
            //LCD_ST7735S_Update();
            HAL_Delay(2000);  /// need this time for reset and reinit USB
            cli_set_first_in_cli(false);
            //LCD_ST7735S_Clear();
            //LCD_ST7735S_Update();

        }
    }
    if (Power_status.vbus_type == BQ2589X_VBUS_NONE && usb_enable == true)
    {
        MX_USB_DEVICE_DeInit();
        usb_enable = false;
        //LCD_ST7735S_Clear();
        //LCD_ST7735_DrawString("USB disconnected",15,35, &Font_8x10, ST7735_WHITE);
        //LCD_ST7735S_Update();
        HAL_Delay(2000);
        cli_set_first_in_cli(true);
        //LCD_ST7735S_Clear();
        //LCD_ST7735S_Update();
#if 0
        bq2589x_enter_ship_mode();
        bq2589x_enter_hiz_mode();
#endif

//        PWR->CSR |= PWR_CSR_EWUP;
//        PWR->CR  |= PWR_CR_CWUF;
//        PWR->CR = PWR_CR_PDDS | PWR_CR_CWUF;
//        HAL_PWR_EnterSTANDBYMode();
    }


}

Power_Status_t *Power_GetStatus(void)
{
    return &Power_status;
}
