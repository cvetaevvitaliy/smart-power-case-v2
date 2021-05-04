#ifndef SMART_POWER_CASE_V2_BATTERY_H
#define SMART_POWER_CASE_V2_BATTERY_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    float temperature;
    uint16_t capacity;
    uint16_t capacity_full;
    uint16_t design_capacity;
    float vbat;
    uint16_t percent;
    uint16_t percent_unfiltered;
    int16_t current;
    int16_t power;
    uint8_t health;
    bool charge_detect;
    bool fast_charge;
    bool battery_discharging;

} Battery_status_t;

void Battery_Init(void);

void Battery_Loop(void);

Battery_status_t *Battery_GetStatus(void);

#endif //SMART_POWER_CASE_V2_BATTERY_H
