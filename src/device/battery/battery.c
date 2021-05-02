#include "battery.h"
#include "bq27441.h"
#include "stm32f4xx.h"
#include "cli.h"
#include "gui_main_screen.h"
#include "eeprom.h"

extern I2C_HandleTypeDef hi2c1;
static uint8_t tmp = 100;

static Battery_status_t Battery_status = {0};


/**
    Read a specified number of bytes over I2C at a given subAddress

    @param subAddress is the 8-bit address of the data to be read
           dest is the data buffer to be written to
           count is the number of bytes to be read
    @return true on success
*/
static int16_t BQ27441_i2cReadBytes(uint8_t DevAddress, uint8_t subAddress, uint8_t * dest, uint8_t count)
{
    if (HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(DevAddress << 1), subAddress, 1, dest, count, 50) == HAL_OK)
        return true;
    else
        return false;
}

/**
    Write a specified number of bytes over I2C to a given subAddress

    @param subAddress is the 8-bit address of the data to be written to
           src is the data buffer to be written
           count is the number of bytes to be written
    @return true on success
*/
static int16_t BQ27441_i2cWriteBytes(uint8_t DevAddress, uint8_t subAddress, uint8_t * src, uint8_t count)
{
    if (HAL_I2C_Mem_Write(&hi2c1, (uint16_t)(DevAddress << 1), subAddress, 1, src, count, 50) == HAL_OK)
        return true;
    else
        return false;
}

static BQ27441_ctx_t BQ27441 =
        {
            .read_reg = BQ27441_i2cReadBytes,
            .write_reg = BQ27441_i2cWriteBytes,
            .BQ27441_i2c_address = BQ72441_I2C_ADDRESS,
        };


void Battery_Init(void)
{
    eepromData_t *settings = eeprom_GetSettings();

    BQ27441_init(&BQ27441);
    BQ27441_enterConfig (true);
    BQ27441_setCapacity((uint16_t)settings->batMah);
    BQ27441_exitConfig (true);
}

void Battery_Loop(void)
{

    ULOG_TRACE("BQ27441 deviceType 0x%04X\n", BQ27441_deviceType());
    Battery_status.temperature = (float) BQ27441_temperature(BATTERY) / 100.0f;
    Battery_status.capacity = BQ27441_capacity(REMAIN_F);
    Battery_status.capacity_full = BQ27441_capacity(FULL_F); // FULL_F
    Battery_status.design_capacity = BQ27441_capacity(DESIGN);
    Battery_status.vbat = (float) BQ27441_voltage() / 1000.0f;
    Battery_status.percent = BQ27441_soc(FILTERED);
    Battery_status.percent_unfiltered = BQ27441_soc(UNFILTERED);
    Battery_status.current = BQ27441_current(AVG);
    Battery_status.power = BQ27441_power();
    Battery_status.health = BQ27441_soh(PERCENT);
    Battery_status.charge_detect = BQ27441_fcFlag();
    Battery_status.fast_charge = BQ27441_chgFlag();
    Battery_status.battery_discharging = BQ27441_dsgFlag();

    ULOG_TRACE("temperature: %.2f\r\n", Battery_status.temperature);
    ULOG_TRACE("capacity: %dmAh\r\n", Battery_status.capacity);
    ULOG_TRACE("capacity_full: %dmAh\r\n", Battery_status.capacity_full);
    ULOG_TRACE("design_capacity: %dmAh\r\n", Battery_status.design_capacity);
    ULOG_TRACE("vbat: %.2fV\r\n", Battery_status.vbat);
    ULOG_TRACE("percent: %d%%\r\n", Battery_status.percent);
    ULOG_TRACE("percent_unfiltered: %d%%\r\n", Battery_status.percent_unfiltered);

    ULOG_TRACE("current: %dmA\r\n", Battery_status.current);
    ULOG_TRACE("power: %dmW\r\n", Battery_status.power);
    ULOG_TRACE("health: %d%%\r\n", Battery_status.health);
    ULOG_TRACE("charge_detect: %d\r\n", Battery_status.charge_detect);

    /** For test position different value to LCD screen*/
    static uint8_t tmp = 100;
    tmp--;

    if (tmp == 0)
        tmp = 100;

    //Battery_status.percent = tmp;
}

Battery_status_t *Battery_GetStatus(void)
{
    return &Battery_status;
}
