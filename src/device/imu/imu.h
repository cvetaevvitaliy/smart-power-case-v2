/**
 * @file imu.h
 * This is part of the project - Smart Power case V2 
 * Copyright (c) Vitaliy Nimych <vitaliy.nimych@gmail.com>
 * Created 28.03.2021
 * SPDX-License-Identifier: GNU General Public License v3.0
 * */

#ifndef SMART_POWER_CASE_V2_IMU_H
#define SMART_POWER_CASE_V2_IMU_H
#include "hw_config.h"
#include "stdbool.h"
#include "stdint.h"

int imu_Init(void);

void imu_Loop(void);

void imu_PowerOff(void);

void imu_InitWakeUp(void);

#endif //SMART_POWER_CASE_V2_IMU_H
