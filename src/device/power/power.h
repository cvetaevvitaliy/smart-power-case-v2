#ifndef SMART_POWER_CASE_V2_POWER_H
#define SMART_POWER_CASE_V2_POWER_H
#include "device_config.h"
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


bool Power_InitChargerChip(void);

void Power_LoopService(void);

Power_Status_t *Power_GetStatus(void);

#endif //SMART_POWER_CASE_V2_POWER_H
