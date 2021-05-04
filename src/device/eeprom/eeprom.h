/**
 * @file eeprom.h
 * This is part of the project - smart-power-case-v2
 * Copyright (c) Vitaliy Nimych <vitaliy.nimych@gmail.com>
 * Created 02.05.2021
 * SPDX-License-Identifier: GNU General Public License v3.0
 * */

#ifndef SMART_POWER_CASE_V2_EEPROM_H
#define SMART_POWER_CASE_V2_EEPROM_H
#include <stdint.h>

#define EEPROM_GOOD_DATA        (65U)

typedef struct {
    uint32_t goodData;
    uint32_t batMah;
    uint32_t chargeCurrent;
    uint32_t batMinVolt;
    uint32_t timerOff;
    uint32_t buzzerState;
    uint32_t accState;

} eepromData_t;


eepromData_t *eeprom_GetHandler(void);

eepromData_t *eeprom_GetSettings(void);

void eeprom_SaveSettings(eepromData_t *data);

void eeprom_ResetToDefaults(void);

void eeprom_CliCommand(void);


#endif //SMART_POWER_CASE_V2_EEPROM_H
