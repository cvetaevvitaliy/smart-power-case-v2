#ifndef SMART_POWER_CASE_V2_POWER_H
#define SMART_POWER_CASE_V2_POWER_H
#include "hw_config.h"
#include "stdbool.h"
#include "stdint.h"
#include "bq2589x_charger.h"


typedef struct
{
    uint16_t vbat;
    uint16_t vbus;
    bq2589x_vbus_type vbus_type;
    bool charge_done;
    uint8_t charging_status;

} Power_Status_t;

typedef enum {
    POWER_OFF_ALL = 0,
    POWER_OFF_MCU,
    POWER_OFF_LCD,
    POWER_OFF_ACC,
    POWER_OFF_CHARGE_CHIP,
} Power_Off_e;


bool Power_InitChargerChip(void);

void Power_Loop(void);

Power_Status_t *Power_GetStatus(void);
void Power_PowerOff(Power_Off_e type);

#endif //SMART_POWER_CASE_V2_POWER_H
