/**
 * @file eeprom.c
 * This is part of the project - smart-power-case-v2
 * Copyright (c) Vitaliy Nimych <vitaliy.nimych@gmail.com>
 * Created 02.05.2021
 * SPDX-License-Identifier: GNU General Public License v3.0
 * */

#include "eeprom.h"
#include "stm32_init.h"
#include "stm32f4xx.h"
#include "cli.h"

static eepromData_t eepromData;

eepromData_t *eeprom_GetHandler(void)
{
    return &eepromData;
}

eepromData_t *eeprom_GetSettings(void)
{
    RTC_HandleTypeDef *rtc = STM32_GetRtcHandle();

    eepromData.goodData = HAL_RTCEx_BKUPRead(rtc, RTC_BKP_DR1);
    eepromData.batMah = HAL_RTCEx_BKUPRead(rtc, RTC_BKP_DR2);
    eepromData.chargeCurrent = HAL_RTCEx_BKUPRead(rtc, RTC_BKP_DR3);
    eepromData.timerOff = HAL_RTCEx_BKUPRead(rtc, RTC_BKP_DR4);
    eepromData.buzzerState = HAL_RTCEx_BKUPRead(rtc, RTC_BKP_DR5);
    eepromData.batMinVolt = HAL_RTCEx_BKUPRead(rtc, RTC_BKP_DR6);
    eepromData.accState = HAL_RTCEx_BKUPRead(rtc, RTC_BKP_DR7);

    if (eepromData.goodData != EEPROM_GOOD_DATA)
        eeprom_ResetToDefaults();

    return &eepromData;

}

void eeprom_SaveSettings(eepromData_t *data)
{
    RTC_HandleTypeDef *rtc = STM32_GetRtcHandle();

    data->goodData = EEPROM_GOOD_DATA;
    HAL_RTCEx_BKUPWrite(rtc, RTC_BKP_DR1, data->goodData);
    HAL_RTCEx_BKUPWrite(rtc, RTC_BKP_DR2, data->batMah);
    HAL_RTCEx_BKUPWrite(rtc, RTC_BKP_DR3, data->chargeCurrent);
    HAL_RTCEx_BKUPWrite(rtc, RTC_BKP_DR4, data->timerOff);
    HAL_RTCEx_BKUPWrite(rtc, RTC_BKP_DR5, data->buzzerState);
    HAL_RTCEx_BKUPWrite(rtc, RTC_BKP_DR6, data->batMinVolt);
    HAL_RTCEx_BKUPWrite(rtc, RTC_BKP_DR7, data->accState);
}


static CLI_Result_t printEepromSettings(void)
{
    CLI_PRINTF("\r\n");
    eepromData_t *data = eeprom_GetSettings();

    CLI_PRINTF("Capacity = %d maH\r\n", data->batMah);
    CLI_PRINTF("Current charge = %d mA\r\n", data->chargeCurrent);
    CLI_PRINTF("Timer auto-Off = %d min\r\n", data->timerOff);
    CLI_PRINTF("Buzzer state = %s\r\n", data->buzzerState ? "Enable" : "Disable");
    CLI_PRINTF("Bat min volt = %d V\r\n", data->batMinVolt);
    CLI_PRINTF("Double ACC click event = %d V\r\n", data->accState);
    return CLI_OK;
}

void eeprom_CliCommand(void)
{
    cli_add_new_cmd("eeprom", printEepromSettings, 0, 0, "Get eeprom data");
}

void eeprom_ResetToDefaults(void)
{

    eepromData.batMah = 5000;
    eepromData.chargeCurrent = 2000;
    eepromData.timerOff = 0;
    eepromData.buzzerState = 1;
    eepromData.batMinVolt = 2800;
    eepromData.accState = 0;

    eeprom_SaveSettings(&eepromData);

}

