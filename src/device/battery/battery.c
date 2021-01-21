#include "battery.h"
#include "bq27441.h"
#include "stm32f4xx.h"
#include "cli.h"

Battery_status_t Battery_status;

void Battery_Init(void)
{
    BQ27441_init();

}

void Battery_LoopService(void)
{
    static uint32_t tv_delay = 0;

    if (HAL_GetTick() - tv_delay > 500)
    {
        Battery_status.temperature = (float)BQ27441_temperature(BATTERY) / 100.0f;
        Battery_status.capacity = BQ27441_capacity(REMAIN_F);
        Battery_status.capacity_full = BQ27441_capacity(FULL_F); // FULL_F
        Battery_status.design_capacity = BQ27441_capacity(DESIGN);
        Battery_status.vbat = (float)BQ27441_voltage() / 1000.0f;
        Battery_status.percent = BQ27441_soc(FILTERED);
        Battery_status.percent_unfiltered = BQ27441_soc(UNFILTERED);
        Battery_status.current = BQ27441_current(AVG);
        Battery_status.power = BQ27441_power();
        Battery_status.health = BQ27441_soh(PERCENT);
        Battery_status.charge_detect = BQ27441_fcFlag();
        Battery_status.fast_charge = BQ27441_chgFlag();
        Battery_status.battery_discharging = BQ27441_dsgFlag();
        tv_delay = HAL_GetTick();

        ULOG_TRACE("temperature: %.2f\n",Battery_status.temperature );
        ULOG_TRACE("capacity: %d\n",Battery_status.capacity );
        ULOG_TRACE("capacity_full: %d\n",Battery_status.capacity_full );
        ULOG_TRACE("design_capacity: %d\n",Battery_status.design_capacity );
        ULOG_TRACE("vbat: %.2f\n",Battery_status.vbat );
        ULOG_TRACE("percent: %d\n",Battery_status.percent );
        ULOG_TRACE("percent_unfiltered: %d\n",Battery_status.percent_unfiltered );

        ULOG_TRACE("current: %d\n",Battery_status.current );
        ULOG_TRACE("power: %d\n",Battery_status.power );
        ULOG_TRACE("health: %d\n",Battery_status.health );
        ULOG_TRACE("charge_detect: %d\n",Battery_status.charge_detect );
    }

}

Battery_status_t *Battery_GetStatus(void)
{
    return &Battery_status;
}
