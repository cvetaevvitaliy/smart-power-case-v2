/**
 * @file imu.h
 * This is part of the project - Smart Power case V2 
 * Copyright (c) Vitaliy Nimych <vitaliy.nimych@gmail.com>
 * Created 28.03.2021
 * SPDX-License-Identifier: GNU General Public License v3.0
 * */

#ifndef SMART_POWER_CASE_V2_IMU_H
#define SMART_POWER_CASE_V2_IMU_H
#include "device_config.h"
#include "stdbool.h"
#include "stdint.h"

int imu_init(void);

void imu_loop(void);

#endif //SMART_POWER_CASE_V2_IMU_H
